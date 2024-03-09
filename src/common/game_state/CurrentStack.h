#ifndef TICHU_CURRENTSTACK_H
#define TICHU_CURRENTSTACK_H


#include <string>
#include "./CardCollection/PlayedCombo.h"
#include "../serialization/uuid_generator.h"
#include "../../../rapidjson/include/rapidjson/document.h"
#include "../serialization/unique_serializable.h"
#include "../serialization/serializable_value.h"

class CurrentStack : public unique_serializable {
private:
    serializable_value<int>* _points;
    PlayedCombo _cards;

// Deserialization Constructor
    CurrentStack(std::string id,
           serializable_value<int>* points,
           PlayedCombo cards);

public:

// Public Constructor
    CurrentStack();  

// Destructor
    ~CurrentStack();

// Accessors
    int get_points(); // Returns points
    std::vector<Card> get_cards(); // Returns the cards 

// State Changes
    void reset(); // Resets points to zero and clears the PlayedCombo
    bool play(CardCollection card_collection); // Give sthe possibilty to playe a CardCollection ontop of this classes PlayedCombo

// Serialization & Deserialization
    static CurrentStack from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};

#endif //TICHU_CURRENTSTACK_H