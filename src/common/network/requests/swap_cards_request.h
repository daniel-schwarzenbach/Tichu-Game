#ifndef TICHU_SWAP_CARDS_REQUEST_H
#define TICHU_SWAP_CARDS_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../game_state/CardCollection/CardCollection.h"

class swap_cards_request : public client_request {

private:

    rapidjson::Value* _card_collection;

    //Private constructor for deserialization
    swap_cards_request(base_class_properties, rapidjson::Value* card_collection);

public:
    // Getter Function
    [[nodiscard]] rapidjson::Value* get_cards() const { return this->_card_collection; }

    // Public Constructor
    swap_cards_request(std::string game_id, std::string player_id, CardCollection card_collection);
    
    // Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static swap_cards_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_SWAP_CARDS_REQUEST_H
