#ifndef TICHU_TEAM_H
#define TICHU_TEAM_H

#include <vector>
#include <string>
#include "../serialization/uuid_generator.h"
#include "../../../rapidjson/include/rapidjson/document.h"
#include "../serialization/unique_serializable.h"
#include "../serialization/serializable_value.h"

class Team : public unique_serializable {
private:
    serializable_value<std::string>* _teamID;
    std::vector<serializable_value<int>>  _points;

// Deserialization Constructor
    Team(std::string id,
            serializable_value<std::string>* teamID,
            std::vector<serializable_value<int>>  points);

public:

// Constructor
    Team(std::string teamID);  

// Destructor
    ~Team();

// Accessors
    int get_points(); // Returns the points summed up 
    std::vector<serializable_value<int>> get_points_vector(); // Returns the points vector
    std::string get_teamID(); // Returns team_id

// State Changes
    void add_points(int points, int round_counter); // Changes the points the team has

// Serialization & Deserialization
    static Team from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};

#endif //TICHU_TEAM_H