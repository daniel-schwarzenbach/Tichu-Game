#ifndef TICHU_CARD_H
#define TICHU_CARD_H

#include <string>
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class Card : public unique_serializable {
private:
    serializable_value<int>* _type;
    serializable_value<int>* _family;

// Deserialization Constructor
    Card(std::string id, serializable_value<int>* type, serializable_value<int>* family);

public:
// Public Constructors
    Card(int type, int family);
    Card();

// Deconstructor
    ~Card();

// Accessors
    int get_type() const noexcept; // Return type (normal values from 2 - 14, Value of 1 or 15 is possible for special cards, 0 would be a placeholder card)
    int get_family() const noexcept; // Return family (between 1 - 4, or 0 if it's a placeholder)
    bool isEmptyCard(); // Check if this card is a placeholder (type 0, family 0)

// Serialization & Deserialization
    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static Card from_json(const rapidjson::Value& json);
    bool operator==(Card const& other) const;
    bool operator!=(Card const& other) const;
};

static const Card phoenix(15,3);
static const Card mah_jong(1,1);
static const Card dragon(15,2);
static const Card dog(15,4);
// 14 Ace
// 13 King
// 12 Queen
// 11 Junge


#endif //TICHU_CARD_H
