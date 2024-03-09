#include "new_game_request.h"


// Public constructor
new_game_request::new_game_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::new_game, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ }

// private constructor for deserialization
new_game_request::new_game_request(client_request::base_class_properties props) :
        client_request(props)
{ }

new_game_request* new_game_request::from_json(const rapidjson::Value& json) {
    return new new_game_request(client_request::extract_base_class_properties(json));
}

void new_game_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
