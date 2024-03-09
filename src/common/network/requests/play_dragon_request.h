#ifndef TICHU_PLAY_DRAGON_REQUEST_H
#define TICHU_PLAY_DRAGON_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../game_state/CardCollection/CardCollection.h"

class play_dragon_request : public client_request {

private:

    rapidjson::Value* _card_collection;
    std::string _other_player_id;

    //Private constructor for deserialization
    play_dragon_request(base_class_properties, rapidjson::Value* card_collection, std::string otherPlayerID);

public:
    // Getter Function
    [[nodiscard]] rapidjson::Value* get_cards() const { return this->_card_collection; }
    [[nodiscard]] std::string get_otherPlayerID() const { return this->_other_player_id; }

    // Public Constructor
    play_dragon_request(std::string game_id, std::string player_id, CardCollection cards, std::string otherPlayerID);
    
    // Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static play_dragon_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_PLAY_DRAGON_REQUEST_H

