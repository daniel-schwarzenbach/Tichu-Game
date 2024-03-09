#ifndef TICHU_EXIT_GAME_REQUEST_H
#define TICHU_EXIT_GAME_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class exit_game_request : public client_request{

private:

    /*
     * Private constructor for deserialization
     */
    explicit exit_game_request(base_class_properties);

public:
    exit_game_request(std::string game_id, std::string player_id);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static exit_game_request* from_json(const rapidjson::Value& json);
};

#endif //TICHU_EXIT_GAME_REQUEST_H