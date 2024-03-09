#include "play_dragon_request.h"

// Public constructor
play_dragon_request::play_dragon_request(std::string game_id, std::string player_id, CardCollection card_collection, std::string other_player_id)
        : client_request(client_request::create_base_class_properties(RequestType::play_dragon, uuid_generator::generate_uuid_v4(), player_id, game_id)),
        _other_player_id(other_player_id)
{ 
    _card_collection = card_collection.to_json();
}

// private constructor for deserialization
play_dragon_request::play_dragon_request(client_request::base_class_properties props, rapidjson::Value* card_collection, std::string other_player_id) :
        client_request(props),
        _card_collection(card_collection),
        _other_player_id(other_player_id) 
{ }

// Deserialization
play_dragon_request* play_dragon_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("cards") && json.HasMember("other_player_id")) {
        return new play_dragon_request(props, json_utils::clone_value(json["cards"].GetObject()), json["other_player_id"].GetString());
    } else {
        throw TichuException("Could not find 'cards' or 'other_player_id' in play_dragon_request");
    }
}

// Serialization
void play_dragon_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("cards", *_card_collection, allocator);
    json.AddMember("other_player_id", _other_player_id, allocator);
}
