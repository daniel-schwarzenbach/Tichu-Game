#ifndef TICHU_DRAW_CARD_REQUEST_H
#define TICHU_DRAW_CARD_REQUEST_H

#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class get_cards_request : public client_request {

private:
    int _nof_cards;

    /*
     * Private constructor for deserialization
     */
    get_cards_request(base_class_properties, int val);

public:

    [[nodiscard]] int get_nof_cards() const { return this->_nof_cards; }

    get_cards_request(std::string game_id, std::string player_id, int nof_cards = 1);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static get_cards_request* from_json(const rapidjson::Value& json);
};


#endif //TICHU_DRAW_CARD_REQUEST_H
