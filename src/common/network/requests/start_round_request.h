#ifndef TICHU_START_ROUND_REQUEST_H
#define TICHU_START_ROUND_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class start_round_request : public client_request {

private:
    
    //Private constructor for deserialization
    start_round_request(base_class_properties);

public:
    // Public constructor to start the game
    start_round_request(std::string game_id, std::string player_id);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static start_round_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_START_ROUND_REQUEST_H
