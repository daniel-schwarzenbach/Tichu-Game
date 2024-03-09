#include "pass_request.h"


// Public constructor
pass_request::pass_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::pass, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ }

// private constructor for deserialization
pass_request::pass_request(client_request::base_class_properties props) :
        client_request(props)
{ }

pass_request* pass_request::from_json(const rapidjson::Value& json) {
    return new pass_request(client_request::extract_base_class_properties(json));
}

void pass_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
