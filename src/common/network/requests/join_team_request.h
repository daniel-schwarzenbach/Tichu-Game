#ifndef TICHU_JOIN_TEAM_REQUEST_H
#define TICHU_JOIN_TEAM_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class join_team_request : public client_request {

private:
    std::string _team;
    
    //Private constructor for deserialization
    join_team_request(base_class_properties, std::string team);

public:
    [[nodiscard]] std::string get_team() const { return this->_team; }

    // Constructor to join a specific team
    join_team_request(std::string game_id, std::string player_id, std::string team);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static join_team_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_JOIN_TEAM_REQUEST_H
