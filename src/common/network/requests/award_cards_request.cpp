#include "award_cards_request.h"

// private constructor for deserialization
award_cards_request::award_cards_request(client_request::base_class_properties props, rapidjson::Value* card_collection, std::string target_player) :
        client_request(props),
        _card_collection(card_collection),
        _target_player(target_player)
{ }

// Public constructor
award_cards_request::award_cards_request(std::string game_id, std::string player_id, CardCollection card_collection, std::string target_player_id)
        : client_request(client_request::create_base_class_properties(RequestType::award_cards, uuid_generator::generate_uuid_v4(), player_id, game_id) ),
        _target_player(target_player_id)
{
    _card_collection = card_collection.to_json();
}

award_cards_request* award_cards_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("cards") && json.HasMember("target_player_id")) {
        return new award_cards_request(props, json_utils::clone_value(json["cards"].GetObject()), json["target_player_id"].GetString());
    } else {
        throw TichuException("Could not find 'cards' or 'target_player_id' in award_cards_request");
    }
}

void award_cards_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("cards", *_card_collection, allocator);
    rapidjson::Value target_player_val(_target_player.c_str(), allocator);
    json.AddMember("target_player_id", target_player_val, allocator);
}
