//
// Created by Manuel on 28.01.2021.
//

#include "client_request.h"

// Include subclasses of class client_request
#include "award_cards_request.h"
#include "bomb_request.h"
#include "exit_game_request.h"
#include "get_cards_request.h"
#include "join_game_request.h"
#include "join_team_request.h"
#include "new_game_request.h"
#include "pass_request.h"
#include "play_cards_request.h"
#include "play_dragon_request.h"
#include "start_game_request.h"
#include "start_round_request.h"
#include "swap_cards_request.h"
#include "tichu_request.h"

#include <iostream>

// for deserialization
const std::unordered_map<std::string, RequestType> client_request::_string_to_request_type = {
    {"bomb", RequestType::bomb},
    {"exit_game", RequestType::exit_game},
    {"get_cards", RequestType::get_cards},
    {"join_game", RequestType::join_game},
    {"join_team", RequestType::join_team},
    {"new_game", RequestType::new_game},
    {"pass", RequestType::pass},
    {"play_cards", RequestType::play_cards},
    {"play_dragon", RequestType::play_dragon},
    {"start_game", RequestType::start_game},
    {"start_round", RequestType::start_round},
    {"swap_cards", RequestType::swap_cards},
    {"tichu", RequestType::tichu}        
};

// for serialization
const std::unordered_map<RequestType, std::string> client_request::_request_type_to_string = {
    {RequestType::bomb, "bomb"},
    {RequestType::exit_game, "exit_game"},
    {RequestType::get_cards, "get_cards"},
    {RequestType::join_game, "join_game"},
    {RequestType::join_team, "join_team"},
    {RequestType::new_game, "new_game"},
    {RequestType::pass, "pass"},
    {RequestType::play_cards, "play_cards"},
    {RequestType::play_dragon, "play_dragon"},
    {RequestType::start_game, "start_game"},
    {RequestType::start_round, "start_round"},
    {RequestType::swap_cards, "swap_cards"},
    {RequestType::tichu, "tichu"}
};

// protected constructor. only used by subclasses
client_request::client_request(client_request::base_class_properties props) :
        _type(props._type),
        _req_id(props._req_id),
        _player_id(props._player_id),
        _game_id(props._game_id)
{ }


// used by subclasses to retrieve information from the json stored by this superclass
client_request::base_class_properties client_request::extract_base_class_properties(const rapidjson::Value& json) {
    if (json.HasMember("player_id") && json.HasMember("game_id") && json.HasMember("req_id")) {
        std::string player_id = json["player_id"].GetString();
        std::string game_id = json["game_id"].GetString();
        std::string req_id = json["req_id"].GetString();
        return create_base_class_properties(
                client_request::_string_to_request_type.at(json["type"].GetString()),
                req_id,
                player_id,
                game_id
        );
    }
    else
    {
        throw TichuException("Client Request did not contain player_id, game_id or req_id");
    }
}

client_request::base_class_properties client_request::create_base_class_properties(
        RequestType type,
        std::string req_id,
        std::string& player_id,
        std::string& game_id)
{
    client_request::base_class_properties res;
    res._player_id = player_id;
    res._game_id = game_id;
    res._req_id = req_id;
    res._type = type;
    return res;
}


void client_request::write_into_json(rapidjson::Value &json,
                                     rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    rapidjson::Value type_val(_request_type_to_string.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    rapidjson::Value player_id_val(_player_id.c_str(), allocator);
    json.AddMember("player_id", player_id_val, allocator);

    rapidjson::Value game_id_val(_game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);

    rapidjson::Value req_id_val(_req_id.c_str(), allocator);
    json.AddMember("req_id", req_id_val, allocator);
}

client_request* client_request::from_json(const rapidjson::Value &json) {
    if (json.HasMember("type") && json["type"].IsString()) {
        const std::string type = json["type"].GetString();
        const RequestType request_type = client_request::_string_to_request_type.at(type);

        // Check which type of request it is and call the respective from_json constructor
        switch(request_type){
            case RequestType::award_cards:
                return award_cards_request::from_json(json);
            case RequestType::bomb:
                return bomb_request::from_json(json);
            case RequestType::exit_game:
                return exit_game_request::from_json(json);
            case RequestType::get_cards:
                return get_cards_request::from_json(json);
            case RequestType::join_game:
                return join_game_request::from_json(json);
            case RequestType::join_team:
                return join_team_request::from_json(json);
            case RequestType::new_game:
                return new_game_request::from_json(json);
            case RequestType::pass:
                return pass_request::from_json(json);
            case RequestType::play_cards:
                return play_cards_request::from_json(json);
            case RequestType::play_dragon:
                return play_dragon_request::from_json(json);
            case RequestType::start_game:
                return start_game_request::from_json(json);
            case RequestType::start_round:
                return start_round_request::from_json(json);
            case RequestType::swap_cards:
                return swap_cards_request::from_json(json);
            case RequestType::tichu:
                return tichu_request::from_json(json);
            default:
                throw TichuException("Encountered unknown ClientRequest type " + type);
        }
    }
    throw TichuException("Could not determine type of ClientRequest. JSON was:\n" + json_utils::to_string(&json));
}


std::string client_request::to_string() const {
    return "client_request of type " + client_request::_request_type_to_string.at(_type) + " for playerId " + _player_id + " and gameId " + _game_id;
}






