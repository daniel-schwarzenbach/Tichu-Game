#include "Player.h"
#include "../exceptions/TichuException.h"

// Constructor for Deserialization
Player::Player( std::string id, serializable_value<std::string>* name, 
                serializable_value<std::string>* teamID, serializable_value<std::string>* gameID,
                serializable_value<int>* points, CardCollection hand, 
                serializable_value<bool>* small_tichu, serializable_value<bool>* big_tichu,
                serializable_value<bool>* has_swapped) :
        unique_serializable(id),
        _points(points),
        _hand(hand),
        _small_tichu(small_tichu),
        _big_tichu(big_tichu),
        _name(name),
        _teamID(teamID),
        _gameID(gameID),
        _has_swapped(has_swapped)
{ }

// Constructors
Player::Player() : unique_serializable() {
    this->_points = new serializable_value<int>(0);
    this->_hand = CardCollection();
    this->_small_tichu = new serializable_value<bool>(false);
    this->_big_tichu = new serializable_value<bool>(false);
    this->_has_swapped = new serializable_value<bool>(false);
    this->_name = new serializable_value<std::string>("");
    this->_teamID = new serializable_value<std::string>("");
    this->_gameID = new serializable_value<std::string>("");  
}

Player::Player(std::string name) : unique_serializable() {
    this->_points = new serializable_value<int>(0);
    this->_hand = CardCollection();
    this->_small_tichu = new serializable_value<bool>(false);
    this->_big_tichu = new serializable_value<bool>(false);
    this->_has_swapped = new serializable_value<bool>(false);
    this->_name = new serializable_value<std::string>(name);
    this->_teamID = new serializable_value<std::string>("");
    this->_gameID = new serializable_value<std::string>("");  
}

Player::Player(std::string id, std::string name) : unique_serializable(id) {
    this->_points = new serializable_value<int>(0);
    this->_hand = CardCollection();
    this->_small_tichu = new serializable_value<bool>(false);
    this->_big_tichu = new serializable_value<bool>(false);
    this->_has_swapped = new serializable_value<bool>(false);
    this->_name = new serializable_value<std::string>(name);
    this->_teamID = new serializable_value<std::string>("");
    this->_gameID = new serializable_value<std::string>("");  
}

// Destructor
Player::~Player() { }

// Getters
int Player::get_points() {
    return _points->get_value();
}

bool Player::get_small_tichu() {
    return _small_tichu->get_value();
}

bool Player::get_big_tichu() {
    return _big_tichu->get_value();
}

std::string Player::get_name() {
    return _name->get_value();
}

std::string Player::get_teamID() {
    return _teamID->get_value();
}

std::string Player::get_gameID() {
    return _gameID->get_value();
}

std::vector<Card> Player::get_handcards() {
    return _hand.get_cards();
}

CardCollection Player::get_hand() {
    return _hand;
}

bool Player::get_swap_state() {
    return _has_swapped->get_value();
}


// Setters
void Player::set_gameID(std::string gameID) {
    _gameID = new serializable_value<std::string>(gameID);
}

void Player::set_teamID(std::string teamID) {
    _teamID = new serializable_value<std::string>(teamID);
}

void Player::add_points(int points) {
    _points->set_value(_points->get_value() + points);
}

// State Change
void Player::reset() {
    _has_swapped->set_value(false);
    _points->set_value(0);
    _small_tichu->set_value(false);
    _big_tichu->set_value(false);
    _hand.remove_all_cards();
}

void Player::add_cards(CardCollection cards){
    for(int i = 0; i < cards.get_cards().size(); i++){
        this->_hand.add_card(cards.get_cards().at(i));
    }
}

bool Player::remove_cards(CardCollection cards){
    for(int i = 0; i < cards.get_cards().size(); i++){
        if(!_hand.try_get_card(cards.get_cards().at(i))){ // Check if the player actually has all these cards
            return false;
        }
    }

    // Remove cards from the players hand
    for(int i = 0; i < cards.get_cards().size(); i++){
        _hand.remove_card(cards.get_cards().at(i));
    }

    return true;
}

bool Player::tichu(std::string type, std::string& err){
    if(type == "big tichu" || type == "small tichu") {
        if(_small_tichu->get_value() || _big_tichu->get_value()) { // A player can only announce one Tichu at the same time
            err="Player already announced a tichu previously";
            return false;
        } else {

            if(type == "small tichu"){
                _small_tichu->set_value(true);
                return true;
            }

            if(type == "big tichu"){
                _big_tichu->set_value(true);
                return true;
            }

        }
    }
    
    err="tichu-type must either be 'big tichu' or 'small tichu'";
    return false;
}

bool Player::has_swapped() {
    if(_has_swapped->get_value()) {
        return true;
    } else {
        _has_swapped->set_value(true);
        return false;
    }
}
 
// Serialization
void Player::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

    rapidjson::Value name_val(rapidjson::kObjectType);
    _name->write_into_json(name_val, allocator);
    json.AddMember("name", name_val, allocator);

    rapidjson::Value small_tichu_val(rapidjson::kObjectType);
    _small_tichu->write_into_json(small_tichu_val, allocator);
    json.AddMember("small_tichu", small_tichu_val, allocator);

    rapidjson::Value big_tichu_val(rapidjson::kObjectType);
    _big_tichu->write_into_json(big_tichu_val, allocator);
    json.AddMember("big_tichu", big_tichu_val, allocator);

    rapidjson::Value has_swapped_val(rapidjson::kObjectType);
    _has_swapped->write_into_json(has_swapped_val, allocator);
    json.AddMember("has_swapped", has_swapped_val, allocator);

    rapidjson::Value points_val(rapidjson::kObjectType);
    _points->write_into_json(points_val, allocator);
    json.AddMember("points", points_val, allocator);

    rapidjson::Value hand_val(rapidjson::kObjectType);
    _hand.write_into_json(hand_val, allocator);
    json.AddMember("hand", hand_val, allocator);

    rapidjson::Value teamID_val(rapidjson::kObjectType);
    _teamID->write_into_json(teamID_val, allocator);
    json.AddMember("teamID", teamID_val, allocator);

    rapidjson::Value gameID_val(rapidjson::kObjectType);
    _gameID->write_into_json(gameID_val, allocator);
    json.AddMember("gameID", gameID_val, allocator);
}

// Deserialization
Player Player::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("name")
        && json.HasMember("small_tichu")
        && json.HasMember("big_tichu")
        && json.HasMember("hand")
        && json.HasMember("points")
        && json.HasMember("teamID")
        && json.HasMember("gameID")
        && json.HasMember("has_swapped"))
    {
        return Player(
                json["id"].GetString(),
                serializable_value<std::string>::from_json(json["name"].GetObject()),
                serializable_value<std::string>::from_json(json["teamID"].GetObject()),
                serializable_value<std::string>::from_json(json["gameID"].GetObject()),
                serializable_value<int>::from_json(json["points"].GetObject()),
                CardCollection::from_json(json["hand"].GetObject()),
                serializable_value<bool>::from_json(json["small_tichu"].GetObject()),
                serializable_value<bool>::from_json(json["big_tichu"].GetObject()),
                serializable_value<bool>::from_json(json["has_swapped"].GetObject()));
    } else {
        throw TichuException("Failed to deserialize Player from json. Required json entries were missing.");
    }
}
