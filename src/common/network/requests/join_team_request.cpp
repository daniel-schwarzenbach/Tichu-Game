#include "join_team_request.h"

// Public constructor
join_team_request::join_team_request(std::string game_id, std::string player_id, std::string team)
        : client_request( client_request::create_base_class_properties(RequestType::join_team, uuid_generator::generate_uuid_v4(), player_id, game_id) ),
          _team(team)
{ }

// private constructor for deserialization
join_team_request::join_team_request(client_request::base_class_properties props, std::string team) :
        client_request(props),
        _team(team)
{ }

void join_team_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    rapidjson::Value team_val(_team.c_str(), allocator);
    json.AddMember("team", team_val, allocator);
}

join_team_request* join_team_request::from_json(const rapidjson::Value& json) {
    if (json.HasMember("team")) {
        return new join_team_request(client_request::extract_base_class_properties(json), json["team"].GetString());
    } else {
        throw TichuException("Could not parse join_team_request from json. team is missing.");
    }
}

