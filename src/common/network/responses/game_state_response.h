#ifndef TICHU_GAME_STATE_RESPONSE_H
#define TICHU_GAME_STATE_RESPONSE_H

#include "server_response.h"
#include "../../game_state/GameState.h"

class game_state_response : public server_response {
private:
    rapidjson::Value* _state_json;

    /*
     * Private constructor for deserialization
     */
    game_state_response(base_class_properties props, rapidjson::Value* state_json);

public:

    game_state_response(std::string game_id, const GameState& state);
    ~game_state_response();

    rapidjson::Value* get_state_json() const;

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static game_state_response* from_json(const rapidjson::Value& json);

#ifdef TICHU_CLIENT
    virtual void Process() const override;
#endif
};


#endif //TICHU_GAME_STATE_RESPONSE_H
