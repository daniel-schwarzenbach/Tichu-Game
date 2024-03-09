#include "exit_game_request.h"


// Public constructor
exit_game_request::exit_game_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::exit_game, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ }

// private constructor for deserialization
exit_game_request::exit_game_request(client_request::base_class_properties props) :
        client_request(props)
{ }

exit_game_request* exit_game_request::from_json(const rapidjson::Value& json) {
    return new exit_game_request(client_request::extract_base_class_properties(json));
}

void exit_game_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
