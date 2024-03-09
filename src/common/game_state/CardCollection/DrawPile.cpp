#include "DrawPile.h"
#include "../../serialization/vector_utils.h"
#include "../../exceptions/TichuException.h"
#include <cstdlib>
#include <ctime> 

// Constructor
DrawPile::DrawPile(std::string id, std::vector<Card> cards):
    CardCollection(id, cards)
{ }

DrawPile::DrawPile() : 
    CardCollection()
{ }

DrawPile::DrawPile(std::vector<Card> cards) : 
    CardCollection(cards)
{ }

// Destructor
DrawPile::~DrawPile() { }

// State Change
void DrawPile::initialize(){

    // Add all cards from 2 - 14 from each of the 4 families
    for(int family = 1; family < 5; family++) {
        for(int type = 2; type < 15; type++) {  
            Card card(type, family);
            this->add_card(card);
        }
    }

    this->add_card(Card(1,1)); // Mah Jong
    this->add_card(Card(15,2)); // Dragon
    this->add_card(Card(15,3)); // Phoenix
    this->add_card(Card(15,4)); // Dog

}

CardCollection DrawPile::get_rnd_card(int amount){
    CardCollection cards;
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed for random number creation

    for(int i = 0; i < amount; i++) {
        int rnd_index = std::rand() % this->number_of_cards(); // Create random number between 0 and number of remaining cards in DrawPile.
        
        // Add card to CardCollection destined for output and delete the card from the drawpile
        cards.add_card(_cards.at(rnd_index));
        _cards.erase(_cards.begin()+rnd_index);
    }

    return cards;
}

// Deserialization
DrawPile DrawPile::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards")) {
        std::vector<Card> deserialized_cards = std::vector<Card>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(Card::from_json(serialized_card.GetObject()));
        }
        return DrawPile(json["id"].GetString(), deserialized_cards);
    } else {
        throw TichuException("Could not parse draw_pile from json. 'id' or 'cards' were missing.");
    }
}

// Serialization
void DrawPile::write_into_json(rapidjson::Value &json,
                                   rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
}