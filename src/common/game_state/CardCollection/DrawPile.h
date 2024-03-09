#ifndef TICHU_DRAWPILE_H
#define TICHU_DRAWPILE_H

#include "Card.h"
#include "CardCollection.h"
#include "../../serialization/serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class DrawPile: public CardCollection {
private:

// Private Constructor
    DrawPile(std::string id, std::vector<Card> cards);

public:

// Public Constructors
    DrawPile();
    DrawPile(std::vector<Card> cards);

// Deconstructor
    ~DrawPile();

// State Change
    void initialize(); // Resets the DrawPile to hold all cards from the Tichu game
    CardCollection get_rnd_card(int amount); // returns random cards from this DrawPile

// Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static DrawPile from_json(const rapidjson::Value& json);
};


#endif //TICHU_DRAWPILE_H
