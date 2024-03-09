#include "PlayedCombo.h"
#include "../../serialization/vector_utils.h"
#include "../../exceptions/TichuException.h"


float PlayedCombo::get_single_cart_value() const{
    Card phoenix = Card(15,3);
    if(     _cards.at(0).get_type() == 15 &&
            _cards.at(0).get_family() == 3){
        return _phoenix_value;
    } 
    else {
        return _cards.at(0).get_type();
    }
}

int PlayedCombo::get_street_value(CardCollection const& card_collection) {
    const std::vector<Card> cards = card_collection.get_cards();
    if(!card_collection.try_get_card(phoenix)){
        return cards.at(0).get_type();
    }
    Card Ass0 = Card(14,0);
    Card Ass1 = Card(14,1);
    Card Ass2 = Card(14,2);
    Card Ass3 = Card(14,3);
    if(     try_get_card(Ass0) ||
            try_get_card(Ass1) ||
            try_get_card(Ass2) ||
            try_get_card(Ass3)
    ){
        assert(cards.size() > 0);
        std::vector<Card> phoenix_lowest(cards.size());
        phoenix_lowest.at(0) = 
                Card(cards.at(0).get_type()-1,1);
        for(int i = 1; i < cards.size(); ++i){
            phoenix_lowest.at(i) = cards.at(i-1);
        }
        CardCollection test(phoenix_lowest);
        if(test.is_valid()){
            return phoenix_lowest.at(0).get_type();
        } 
        else{
            return cards.at(0).get_type();
        }
    }
    else{
        return cards.at(0).get_type();
    }
}

// Constructors
PlayedCombo::PlayedCombo(std::vector<Card> cards):
    CardCollection(cards)
{ _phoenix_value = 1.5;}

PlayedCombo::PlayedCombo(std::string id, std::vector<Card> cards):
    CardCollection(id, cards)
{ _phoenix_value = 1.5;}

PlayedCombo::PlayedCombo() : 
    CardCollection()
{ _phoenix_value = 1.5;}

// Destructor
PlayedCombo::~PlayedCombo() { }

// Accessors
bool PlayedCombo::can_be_played(CardCollection card_collection){
    // Assert that the card_collection is valid
    if(!card_collection.is_valid()){ 
        return false;
    }

    Card phoenix = Card(15,3);
    if(!card_collection.try_get_card(phoenix)){

        // If no card is lying on the table you can always play
        if(this->number_of_cards() == 0) { return true; }

        // If the dog card is lying on the table you can always play
        if(_cards.at(0).get_type() == 15 && _cards.at(0).get_family() == 4) { return true; }

        // If no bomb is on the table, you can always play a bomb
        if(!this->is_valid_bomb() && card_collection.is_valid_bomb()) {
            return true;
        }

        // If a Bomb Quad is on the table, you can always play a Bomb Street
        if(this->get_type() == "Bomb_Quad" && card_collection.get_type() == "Bomb_Street") {
            return true;
        }

        // In all other cases the type of the CardCollection in the hand must be the same as the type of the CardCollection on the table.
        if(this->get_type() != card_collection.get_type()) {
            return false;
        }

        // Single
        if(this->get_type() == "Single") {

            // You can't play a dog ontop of another card.
            if (card_collection.get_cards().at(0).get_type() == 15 && card_collection.get_cards().at(0).get_family() == 4){
                return false;
            } 

            // Card must be higher than the one on the desk.
            if(card_collection.get_cards().at(0).get_type() > this->get_single_cart_value()){
                return true;
            } 
        }

        // Doubles
        // Tripple
        // Bomb Quad
        if(this->get_type() == "Doubles" || 
            this->get_type() == "Tripple" ||
            this->get_type() == "Bomb_Quad") {

            // It must be higher than the one on the desk.
            if(card_collection.get_cards().at(0).get_type() > _cards.at(0).get_type()){
                return true;
            } 
        }

        // Fullhouse
        if(this->get_type() == "Fullhouse") {
            int index_cards;
            if(_cards.at(2).get_type() == _cards.at(3).get_type()) {
                // _cards:  Tripple is higher than pair
                index_cards = 2;
            } else {
                // _cards: Pair is higher than Tripple
                index_cards = 0;
            }

            int index_card_collection;
            if(card_collection.get_cards().at(2).get_type() == card_collection.get_cards().at(3).get_type()) {
                // card_collection:  Tripple is higher than pair
                index_card_collection = 2;
            } else {
                // card_collection: Pair is higher than Tripple
                index_card_collection = 0;
            }

            // It must be higher than the one on the desk.
            if(card_collection.get_cards().at(index_card_collection).get_type() > _cards.at(index_cards).get_type()){
                return true;
            }                        
        }

        // Street
        // Following Doubles
        if(this->get_type() == "Street" || this->get_type() == "Following_Doubles") {

            // Must have same length
            if(this->number_of_cards() != card_collection.number_of_cards()) {
                return false;
            }
            if(this->get_type() == "Street"){
                if(get_street_value(card_collection) > get_street_value(*this)){
                    return true;
                }
            } 
            else{
                // It must be higher than the one on the desk.
                if(card_collection.get_cards().at(0).get_type() > _cards.at(0).get_type()){
                    return true;
                }
            }
            
            
        }

        // Bomb Street
        if(this->get_type() == "Bomb_Street") {

            // Must have at least same length
            if(this->number_of_cards() > card_collection.number_of_cards()) {
                return false;
            }

            // Longer bomb is better
            if(this->number_of_cards() < card_collection.number_of_cards()) {
                return true;
            }

            // If bomb hase same lenght, it must be higher than the one on the desk.
            if(card_collection.get_cards().at(0).get_type() > _cards.at(0).get_type()){
                return true;
            }
        }

    } else{ //                                  PHOENIX
        // If no card is lying on the table you can always play
        if(this->number_of_cards() == 0) { 
            _phoenix_value = 1.5;
            return true; 
        }

        // If the dog card is lying on the table you can always play
        if(_cards.at(0).get_type() == 15 && _cards.at(0).get_family() == 4) { return true; }

        // If no bomb is on the table, you can always play a bomb
        if(!this->is_valid_bomb() && card_collection.is_valid_bomb()) {
            return true;
        }

        // In all other cases the type of the CardCollection in the hand must be the same as the type of the CardCollection on the table.
        if(this->get_type() != card_collection.get_type()) {
            return false;
        }

        // Single
        if(this->get_type() == "Single") {
            // You can't play ontop a dragon
            if (_cards.at(0).get_type() == 15 && _cards.at(0).get_family() == 2){
                return false;
            }
            else{
                return true;
            }
            
        }

        // Doubles
        // Tripple
        // Bomb Quad
        if(this->get_type() == "Doubles" || 
            this->get_type() == "Tripple" ||
            this->get_type() == "Bomb_Quad") {

            // It must be higher than the one on the desk.
            if(card_collection.get_cards().at(0).get_type() > _cards.at(0).get_type()){
                return true;
            } 
        }

        // Fullhouse
        if(this->get_type() == "Fullhouse") {
            int index_cards;
            if(_cards.at(3).get_type() == _cards.at(2).get_type()) {
                // _cards:  Tripple is higher than pair
                index_cards = 2;
            } else {
                // _cards: Pair is higher than Tripple
                index_cards = 0;
            }

            int index_card_collection;
            if(card_collection.get_cards().at(2).get_type() == card_collection.get_cards().at(3).get_type()) {
                // card_collection:  Tripple is higher than pair
                index_card_collection = 2;
            } else {
                // card_collection: Pair is higher than Tripple
                index_card_collection = 0;
            }

            // It must be higher than the one on the desk.
            if(card_collection.get_cards().at(index_card_collection).get_type() > _cards.at(index_cards).get_type()){
                return true;
            }                        
        }

        // Street
        // Following Doubles
        if(this->get_type() == "Street" || this->get_type() == "Following_Doubles") {

            // Must have same length
            if(this->number_of_cards() != card_collection.number_of_cards()) {
                return false;
            }

            // It must be higher than the one on the desk.
            if(card_collection.get_cards().at(0).get_type() > _cards.at(0).get_type()){
                return true;
            }
        }

        // Bomb Street
        if(this->get_type() == "Bomb_Street") {

            // Must have at least same length
            if(this->number_of_cards() > card_collection.number_of_cards()) {
                return false;
            }

            // Longer bomb is better
            if(this->number_of_cards() < card_collection.number_of_cards()) {
                return true;
            }

            // If bomb hase same lenght, it must be higher than the one on the desk.
            if(card_collection.get_cards().at(0).get_type() > _cards.at(0).get_type()){
                return true;
            }
        }
    }

    // No checks passed
    return false;   
}

void PlayedCombo::set_cards(CardCollection cards){

    if(     cards.get_cards().size() == 1 && 
            _cards.size() == 1 &&
            cards.get_cards().at(0) == phoenix
    ){
            _phoenix_value = _cards.at(0).get_type() + 0.5f;
        
    }
    _cards = cards.get_cards();
}

// Deserialization
PlayedCombo PlayedCombo::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards")) {
        std::vector<Card> deserialized_cards = std::vector<Card>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(Card::from_json(serialized_card.GetObject()));
        }
        return PlayedCombo(json["id"].GetString(), deserialized_cards);
    } else {
        throw TichuException("Could not parse PlayedCombo from json. 'id' or 'cards' were missing.");
    }
}

// Serialization
void PlayedCombo::write_into_json(rapidjson::Value &json,
                                   rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
}