#ifndef TICHU_PLAYEDCOMBO_H
#define TICHU_PLAYEDCOMBO_H

#include <string>
#include <vector>
#include "Card.h"
#include "CardCollection.h"
#include "../../serialization/serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class PlayedCombo: public CardCollection {
private:

// Deserialization Constructor
    PlayedCombo(std::string id, std::vector<Card> cards);
    float _phoenix_value;
    // returns value of a single laying card with pheonix exeption
    float get_single_cart_value() const;
    // returns value of a single laying card with pheonix exeption
    int get_street_value(CardCollection const& cards);

public:

// Public Constructors
    PlayedCombo(std::vector<Card> cards);
    PlayedCombo();

// Destructor
    ~PlayedCombo();

// Accessors
    bool can_be_played(CardCollection cards); // checks if the specified CardCollection can be played upon this PlayedCombo according to the Tichu ruleset
    void set_cards(CardCollection cards); // updates the cards in this PlayedCombo

// Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static PlayedCombo from_json(const rapidjson::Value& json);
};

#endif //TICHU_PLAYEDCOMBO_H
