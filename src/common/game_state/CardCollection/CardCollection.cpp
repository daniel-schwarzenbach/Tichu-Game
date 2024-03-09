#include "CardCollection.h"
#include "../../serialization/vector_utils.h"
#include "../../exceptions/TichuException.h"

// Constructors
CardCollection::CardCollection(std::vector<Card> cards):
        unique_serializable(),
        _cards(cards)
{ }

CardCollection::CardCollection(std::string id, std::vector<Card> cards):
        unique_serializable(id),
        _cards(cards)
{ }

CardCollection::CardCollection() : unique_serializable() { }

// Destructor
CardCollection::~CardCollection() { }

// Accessors
int CardCollection::number_of_cards() const{
    return _cards.size();
}

std::vector<Card> CardCollection::get_cards() const{
    return _cards;
}

bool CardCollection::try_get_card(Card card) const{
    // Search for the specified card within this CardCollection
    for(int i = 0; i < this->number_of_cards(); i++){
        if(_cards.at(i).get_type() == card.get_type() && _cards.at(i).get_family() == card.get_family()){
            return true;
        }
    }
    return false;
}

int CardCollection::get_points() {
    int points = 0;
    for(int i = 1; i < 5; i++) {
        if(try_get_card(Card(10,i))){ // A card with value 10 is worth 10 points.
            points = points + 10;
        }

        if(try_get_card(Card(13,i))){ // A card with value king  is worth 10 points.
            points = points + 10;
        }

        if(try_get_card(Card(5,i))){ // A card with value 5 is worth 5 points.
            points = points + 5;
        }
    }

    if(try_get_card(Card(15,2))){ // A card with value dragon is worth 25 points.
            points = points + 25;
    }

    if(try_get_card(Card(15,3))){ // A card with value phoenix is worth -25 points.
            points = points - 25;
    }

    return points;
}

// State Change
void CardCollection::add_card(Card card){
    // Add the card to the CardCollection, so that its always ordered by type and by family.
    std::vector<Card>::iterator iter = _cards.begin();
    while(iter != _cards.end()){
        if((*iter).get_type() > card.get_type()){
            break;
        }
        if((*iter).get_type() == card.get_type() && (*iter).get_family() > card.get_family()){
            break;
        }
        
        iter++;
    }
    
    _cards.insert(iter,card);
    
}

bool CardCollection::remove_card(Card card){
    // Search for the specified card and remove it
    for(int i = 0; i < this->number_of_cards(); i++){
        if(_cards.at(i).get_type() == card.get_type() && _cards.at(i).get_family() == card.get_family()){
            _cards.erase(_cards.begin()+i);
            return true;
        }
    }

    // In this case the specified card doesn't exist in this CardCollection
    return false;
}

void CardCollection::remove_all_cards(){
    _cards.clear();
}

bool CardCollection::is_valid(){
    if(get_type() != "") { // As long as the type is not an empty string, the collection is valid
        return true;
    }

    return false;
}

std::string CardCollection::get_type() {

    if((try_get_card(dragon) || try_get_card(dog)) && this->number_of_cards() != 1) { // Dragon and dog can only be played as single cards
        return "";
    } 

    if(!try_get_card(phoenix)){
        bool check;
        switch(this->number_of_cards()){
            case 0:
                // No card isn't a valid CardCollection
                return "";

            case 1:
                // Single Card is always a valid CardCollection
                return "Single";

            case 2:
                // Check for double
                if(_cards.at(0).get_type()==_cards.at(1).get_type()){

                    return "Doubles";
                } 

                return "";
            
            case 3:
                // Check for tripples
                if(_cards.at(0).get_type() == _cards.at(1).get_type() && _cards.at(0).get_type() == _cards.at(2).get_type()){
                    return "Tripple";
                }

                return "";
            
            case 5:
                // Check for full house
                  if(_cards.at(0).get_type() == _cards.at(1).get_type()){
                    if(_cards.at(2).get_type() == _cards.at(3).get_type() && _cards.at(2).get_type() == _cards.at(4).get_type()){
                        return "Fullhouse";
                    }
                } 

                if(_cards.at(0).get_type() == _cards.at(1).get_type() && _cards.at(0).get_type() == _cards.at(2).get_type()){
                    if(_cards.at(3).get_type() == _cards.at(3).get_type()){
                        return "Fullhouse";
                    } 
                }

            default: // To reach this point number_of_cards must be >= 4 and not be a full house. This means it can only be a street, doubles following each other, a bomb or an invalid CardCollection.
                // Check for bomb
                if(_cards.at(0).get_type() == _cards.at(1).get_type() && _cards.at(0).get_type() == _cards.at(2).get_type() && _cards.at(0).get_type() == _cards.at(3).get_type()){
                    return "Bomb_Quad";
                }
                
                // Check for street bomb
                std::vector<Card>::iterator iter;
                iter = _cards.begin();
                check = true;
                
                if(_cards.size() > 4){
                    while (iter+1 != _cards.end()){
                        if(iter->get_type() == (iter+1)->get_type()-1 && iter->get_family() == (iter+1)->get_family()) {  
                            } else{
                                check = false;
                            }
                        iter++;
                    }

                    if(check){
                        return "Bomb_Street";
                    }
                }

                // Check for street
                iter = _cards.begin();
                check = true;
                
                if(_cards.size() > 4){
                    while (iter+1 != _cards.end()){
                        if(iter->get_type() == (iter+1)->get_type()-1) {  
                            } else{
                                check = false;
                            }
                        iter++;
                    }

                    if(check){
                        return "Street";
                    }
                }

                // Check for doubles following each other
                check = true;
                if(number_of_cards() % 2 == 0){ 
                    for(int i=0; i < number_of_cards()/2; i=i+1){
                        if(_cards.at(2*i).get_type() != _cards.at((2*i)+1).get_type()){
                            check = false;
                        }
                        if(_cards.at(0).get_type() + i != _cards.at(2*i).get_type()) {
                            check = false;
                        }
                    }

                    if(check){
                        return "Following_Doubles";
                    }
                }
        } 
    } else{
        // there is a           PHOENIX         !!!
        //bool check;
        switch(this->number_of_cards()){
            case 0:{
                return "";
            }
            case 1:{
                return "Single";
            }
            case 2:{
                if(     _cards.at(0) != mah_jong)
                    return "Doubles";
                else
                    return "";
            }
            case 3:{
                if(_cards.at(0).get_type() == _cards.at(1).get_type()
                        && !try_get_card(mah_jong))
                    return "Tripple";
                else
                    return "";
            }
            case 5:{
                // check full house
                if(     (_cards[0].get_type()==_cards[1].get_type()&&
                        _cards[2].get_type() == _cards[3].get_type())
                        ||
                        (_cards[0].get_type()==_cards[1].get_type()&&
                        _cards[2].get_type() == _cards[1].get_type())
                        ||
                        (_cards[1].get_type()==_cards[2].get_type()&&
                        _cards[2].get_type() == _cards[3].get_type())
                        && !try_get_card(mah_jong)
                ){
                    return "Fullhouse";
                }
            }
            default:{
                // check street_bomb or street
                if(_cards.size() > 4 && (
                (_cards.size() == 14 && _cards.at(0)==mah_jong) ||
                (_cards.size() < 14) )
                ){
                    bool phoenix_usable = true;
                    bool is_street = true;
                    for(auto card = _cards.begin(); card < _cards.end() - 2; ++card) {
                        if(card->get_type()!=(card+1)->get_type()-1){
                            // use Phoenix
                            if(phoenix_usable && card->get_type() == (card+1)->get_type()-2) {
                                phoenix_usable = false;
                            } 
                            else{
                                is_street = false;
                                break;
                            }
                        }
                    }
                    if(is_street){
                        return "Street";
                    }
                }
                // check Following_Doubles
                if(_cards.size()%2 == 0){
                    bool phoenix_usable = true;
                    bool is_following_doubles = true;
                    auto card = _cards.begin()+1;
                    if((card-1)->get_type() != card->get_type()){
                        //try to use phoenix
                        if((card-1)->get_type()== card->get_type()-1  && (card+1)->get_type()== card->get_type()){
                            phoenix_usable = false;
                            card++;
                        } 
                        else{
                            is_following_doubles = false;
                        }
                    }
                    while(card < _cards.end() - 2 && is_following_doubles){
                        bool phoenix_has_been_used = false;
                        // check if next pair is one bigger
                        if(card->get_type()!=(card+1)->get_type()-1){
                            is_following_doubles = false;
                        }
                        // check if next pair exist, else try using 
                        // phoenix
                        if(card->get_type()!=(card+2)->get_type()-1){
                            // try to us phoenix
                            if(phoenix_usable){
                                phoenix_has_been_used = true;
                                phoenix_usable = false;
                            } else{
                                is_following_doubles = false;
                                break;
                            }
                        }
                        // iterate
                        if(phoenix_has_been_used){
                            card++;
                        } else{
                            card += 2;
                        }
                    }
                    if(is_following_doubles){
                        return "Following_Doubles";
                    }
                }
            }
        }   
    }

    // Reached end of function, thus this CardCollection must be invalid
    return "";
}

bool CardCollection::is_valid_bomb(){
    if(get_type() == "Bomb_Quad" || get_type() == "Bomb_Street") {
        return true;
    }
    
    return false;
}

// Deserialization
CardCollection CardCollection::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards")) {
        std::vector<Card> deserialized_cards = std::vector<Card>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(Card::from_json(serialized_card.GetObject()));
        }
        return CardCollection(json["id"].GetString(), deserialized_cards);
    } else {
        throw TichuException("Could not parse CardCollection from json. 'id' or 'cards' were missing.");
    }
}

// Serialization
void CardCollection::write_into_json(rapidjson::Value &json,
                                   rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
}

