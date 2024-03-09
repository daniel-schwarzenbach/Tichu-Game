#ifndef TICHU_PLAY_CARDS_REQUEST_H
#define TICHU_PLAY_CARDS_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../game_state/CardCollection/CardCollection.h"

class play_cards_request : public client_request {

private:

    rapidjson::Value* _card_collection;

    //Private constructor for deserialization
    play_cards_request(base_class_properties, rapidjson::Value* card_collection);

public:
    // Getter Function
    [[nodiscard]] rapidjson::Value* get_cards() const { return this->_card_collection; }

    // Public Constructor
    play_cards_request(std::string game_id, std::string player_id, CardCollection cards);
    
    // Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static play_cards_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_PLAY_CARDS_REQUEST_H

