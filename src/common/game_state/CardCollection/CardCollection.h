#ifndef TICHU_CARDCOLLECTION_H
#define TICHU_CARDCOLLECTION_H

#include <string>
#include <vector>
#include "Card.h"
#include "../../serialization/serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class CardCollection: public unique_serializable {
protected:
    std::vector<Card> _cards;
    
// Deserialization Constructor
    CardCollection(std::string id, std::vector<Card> cards);

public:

// Public Constructor
    CardCollection();
    CardCollection(std::vector<Card> cards);

// Deconstructor
    ~CardCollection();

// Accessors
    int number_of_cards() const; // Returns number of cards in this CardCollection
    std::vector<Card> get_cards() const; // Returns a vector of the cards in this CardCollection
    bool try_get_card(Card card) const; // checks if the specified card is in this CardCollection
    bool is_valid(); // checks if this CardCollection is a valid card combo according to the Tichu ruleset
    std::string get_type(); // Returns the type of the CardCollection {Single, Doubles, Following_Doubles, Tripple, Fullhouse, Street, Bomb_Quad, Bomb_Street}
    bool is_valid_bomb(); // checks if this CardCollection is of type bomb
    std::string get_type_phoenix();
    int get_points(); // Returns the value of this CardCollection

// State Change
    void add_card(Card card); // Adds the specified card to this CardCollection
    bool remove_card(Card card); // Removes the specified card to this CardCollection
    void remove_all_cards(); // Removes all cards from this CardCollection

// Serialization & Deserialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static CardCollection from_json(const rapidjson::Value& json);
};


#endif //TICHU_CARDCOLLECTION_H
