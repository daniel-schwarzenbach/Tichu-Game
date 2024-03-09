#include "tichu_request.h"

// Public constructor
tichu_request::tichu_request(std::string game_id, std::string player_id, std::string tichu)
        : client_request( client_request::create_base_class_properties(RequestType::tichu, uuid_generator::generate_uuid_v4(), player_id, game_id) ),
          _tichu(tichu)
{ }

// private constructor for deserialization
tichu_request::tichu_request(client_request::base_class_properties props, std::string tichu) :
        client_request(props),
        _tichu(tichu)
{ }

void tichu_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    rapidjson::Value tichu_val(_tichu.c_str(), allocator);
    json.AddMember("tichu", tichu_val, allocator);
}

tichu_request* tichu_request::from_json(const rapidjson::Value& json) {
    if (json.HasMember("tichu")) {
        return new tichu_request(client_request::extract_base_class_properties(json), json["tichu"].GetString());
    } else {
        throw TichuException("Could not parse tichu_request from json. tichu is missing.");
    }
}

