#include "Team.h"
#include "../exceptions/TichuException.h"
#include "../serialization/vector_utils.h"

// Constructor for Deserialization
Team::Team( std::string id, 
            serializable_value<std::string>* teamID, 
            std::vector<serializable_value<int>> points) :
        unique_serializable(id),
        _points(points),
        _teamID(teamID)
{ }

// Constructor
Team::Team(std::string teamID) : unique_serializable() {
    this->_points = std::vector<serializable_value<int>>();
    this->_teamID = new serializable_value<std::string>(teamID);  
}

// Destructor
Team::~Team() { }

// Getters
int Team::get_points() {
    int points = 0;
    for(int i = 0; i < _points.size(); i++) {
        points = points + _points.at(i).get_value();
    }
    return points;
}

std::vector<serializable_value<int>>  Team::get_points_vector() {
    return _points;
}

std::string Team::get_teamID() {
    return this->_teamID->get_value();
}

// State Change
void Team::add_points(int points, int round_counter) {
    if(_points.size() < round_counter) {
        _points.push_back(0);
    }
    _points.at(round_counter - 1).set_value(_points.at(round_counter - 1).get_value() + points);
    
}

// Serialization
void Team::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

    json.AddMember("points", vector_utils::serialize_vector(_points, allocator), allocator);

    rapidjson::Value teamID_val(rapidjson::kObjectType);
    _teamID->write_into_json(teamID_val, allocator);
    json.AddMember("teamID", teamID_val, allocator);
}

// Deserialization
Team Team::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("points")
        && json.HasMember("teamID"))
    {   
        std::vector<serializable_value<int>> deserialized_points;
        for (auto &serialized_points : json["points"].GetArray()) {
            deserialized_points.push_back(*serializable_value<int>::from_json(serialized_points.GetObject()));
        }
        return Team(
                json["id"].GetString(),
                serializable_value<std::string>::from_json(json["teamID"].GetObject()),
                deserialized_points);
    } else {
        throw TichuException("Failed to deserialize Team from json. Required json entries were missing.");
    }
}
