#ifndef TICHU_PLAYER_H
#define TICHU_PLAYER_H

#include <string>
#include "./CardCollection/CardCollection.h"
#include "../serialization/uuid_generator.h"
#include "../../../rapidjson/include/rapidjson/document.h"
#include "../serialization/unique_serializable.h"
#include "../serialization/serializable_value.h"

class Player : public unique_serializable {
private:
    serializable_value<int>* _points;
    CardCollection _hand;
    serializable_value<bool>* _small_tichu;
    serializable_value<bool>* _big_tichu;
    serializable_value<bool>* _has_swapped;
    serializable_value<std::string>* _name;
    serializable_value<std::string>* _teamID;
    serializable_value<std::string>* _gameID;

// Deserialization Constructor
    Player(std::string id,
           serializable_value<std::string>* name,
           serializable_value<std::string>* teamID,
           serializable_value<std::string>* gameID,
           serializable_value<int>* points,
           CardCollection hand,
           serializable_value<bool>* small_tichu,
           serializable_value<bool>* big_tichu,
           serializable_value<bool>* has_swapped);

public:

// Constructors
    Player(std::string name);  
    Player(std::string id, std::string name);  
    Player();

// Destructor
    ~Player();

// Accessors
    int get_points(); // Returns players points
    bool get_small_tichu(); // Check if player announced a small tichu
    bool get_big_tichu(); // Check if player announced a big tichu
    std::string get_name(); // Return players name
    std::string get_teamID(); // Return players team id
    std::string get_gameID(); // Return players game id
    std::vector<Card> get_handcards(); // Return players handcards as a vector
    CardCollection get_hand(); // Return players handcards as a CardCollection
    bool get_swap_state(); // Check if player swapped cards before

// State Changes
    void reset(); // Resets the players hand and points to make him ready to start a new round
    void set_gameID(std::string game_id); // Changes the players game_id
    void set_teamID(std::string team_id); // Changes the players team_id
    void add_points(int points); // Changes the players points
    void add_cards(CardCollection cards); // Adds cards to the players hand
    bool remove_cards(CardCollection cards); // Removes cards from the players hand
    bool tichu(std::string type, std::string& err); // Changes the announces Tichu
    bool has_swapped(); // Changes the swap state

// Serialization & Deserialization
    static Player from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};

#endif //TICHU_PLAYER_H