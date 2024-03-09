#ifndef TICHU_TICHU_REQUEST_H
#define TICHU_TICHU_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class tichu_request : public client_request {

private:
    std::string _tichu;
    
    //Private constructor for deserialization
    tichu_request(base_class_properties, std::string tichu);

public:
    [[nodiscard]] std::string get_tichu() const { return this->_tichu; }

    // Constructor to join a specific team
    tichu_request(std::string game_id, std::string player_id, std::string tichu);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static tichu_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_TICHU_REQUEST_H
