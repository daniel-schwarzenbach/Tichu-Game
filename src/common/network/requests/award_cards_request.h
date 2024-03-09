#ifndef TICHU_AWARD_CARDS_REQUEST_H
#define TICHU_AWARD_CARDS_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../game_state/CardCollection/CardCollection.h"

class award_cards_request : public client_request {

private:
    std::string _target_player;
    rapidjson::Value* _card_collection;

    //Private constructor for deserialization
    award_cards_request(base_class_properties, rapidjson::Value* card_collection, std::string target_player_id);

public:
    // Getter Functions
    [[nodiscard]] rapidjson::Value* get_card_collection() const { return this->_card_collection; }
    [[nodiscard]] std::string get_target_player() const { return this->_target_player; }

    // Public Constructor
    award_cards_request(std::string game_id, std::string player_id, CardCollection card_collection, std::string target_player);

    // Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static award_cards_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_AWARD_CARDS_REQUEST_H
