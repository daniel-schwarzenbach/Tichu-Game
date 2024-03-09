#ifndef TICHU_BOMB_REQUEST_H
#define TICHU_BOMB_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../game_state/CardCollection/CardCollection.h"

class bomb_request : public client_request{

private:

    rapidjson::Value* _card_collection;

    // Private constructor for deserialization
    explicit bomb_request(base_class_properties, rapidjson::Value* card_collection);

public:
    // Getter Function
    [[nodiscard]] rapidjson::Value* get_cards() const { return this->_card_collection; }

    // Public Constructor
    bomb_request(std::string game_id, std::string player_id, CardCollection card_collection);

    // Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static bomb_request* from_json(const rapidjson::Value& json);
};

#endif //TICHU_BOMB_REQUEST_H