#ifndef TICHU_PASS_REQUEST_H
#define TICHU_PASS_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class pass_request : public client_request{

private:

    /*
     * Private constructor for deserialization
     */
    explicit pass_request(base_class_properties);

public:
    pass_request(std::string game_id, std::string player_id);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static pass_request* from_json(const rapidjson::Value& json);
};

#endif //TICHU_PASS_REQUEST_H