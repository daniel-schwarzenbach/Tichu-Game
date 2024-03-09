#include "game_state_response.h"

#include "../../exceptions/TichuException.h"
#include "../../serialization/json_utils.h"

#ifdef TICHU_CLIENT
#include "../../../client/GameController.h"
#endif

game_state_response::game_state_response(server_response::base_class_properties props, rapidjson::Value* state_json) :
        server_response(props),
        _state_json(state_json)
{ }

game_state_response::game_state_response(std::string game_id, const GameState& state) :
        server_response(server_response::create_base_class_properties(ResponseType::game_state_msg, game_id))
{
    this->_state_json = state.to_json();
}


void game_state_response::write_into_json(rapidjson::Value &json,
                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    server_response::write_into_json(json, allocator);
    json.AddMember("state_json", *_state_json, allocator);
}

game_state_response *game_state_response::from_json(const rapidjson::Value& json) {
    if (json.HasMember("state_json")) {
        return new game_state_response(server_response::extract_base_class_properties(json),
                                       json_utils::clone_value(json["state_json"].GetObject()));
    } else {
        throw TichuException("Could not parse game_state_response from json. state is missing.");
    }
}

game_state_response::~game_state_response() {
    if (_state_json != nullptr) {
        delete _state_json;
        _state_json = nullptr;
    }
}

rapidjson::Value* game_state_response::get_state_json() const {
    return _state_json;
}

#ifdef TICHU_CLIENT

void game_state_response::Process() const {
    try {
        GameState state = GameState::from_json(*_state_json);
        GameController::updateGameState(state);

    } catch(std::exception& e) {
        std::cerr << "Failed to extract game_state from game_state_response" << std::endl
                  << e.what() << std::endl;
    }
}

#endif
