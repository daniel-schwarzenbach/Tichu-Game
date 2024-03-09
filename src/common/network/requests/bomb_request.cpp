#include "bomb_request.h"


// Public constructor
bomb_request::bomb_request(std::string game_id, std::string player_id, CardCollection card_collection)
        : client_request( client_request::create_base_class_properties(RequestType::bomb, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ 
        _card_collection = card_collection.to_json();
}

// private constructor for deserialization
bomb_request::bomb_request(client_request::base_class_properties props, rapidjson::Value* card_collection) :
        client_request(props),
        _card_collection(card_collection)
{ }

bomb_request* bomb_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("cards")) {
        return new bomb_request(props, json_utils::clone_value(json["cards"].GetObject()));
    } else {
        throw TichuException("Could not find 'cards' in bomb_request");
    }
}

void bomb_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("cards", *_card_collection, allocator);
}