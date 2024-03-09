#include "get_cards_request.h"

// Public constructor
get_cards_request::get_cards_request(std::string game_id, std::string player_id, int nof_cards)
        : client_request( client_request::create_base_class_properties(RequestType::get_cards, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{
    _nof_cards = nof_cards;
}

// private constructor for deserialization
get_cards_request::get_cards_request(client_request::base_class_properties props, int nof_cards) :
        client_request(props),
        _nof_cards(nof_cards)
{ }

get_cards_request* get_cards_request::from_json(const rapidjson::Value &json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("nof_cards") ) {
        return new get_cards_request(props, json["nof_cards"].GetInt());
    } else {
        throw TichuException("Could not find 'nof_cards' in get_cards_request");
    }
}

void get_cards_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("nof_cards", rapidjson::Value(this->_nof_cards),allocator);
}
