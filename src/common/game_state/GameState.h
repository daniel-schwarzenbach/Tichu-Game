#ifndef TICHU_GAMESTATE_H
#define TICHU_GAMESTATE_H

#include <vector>
#include <string>
#include "../../rapidjson/include/rapidjson/document.h"
#include "Player.h"
#include "Team.h"
#include "CurrentStack.h"
#include "CardCollection/DrawPile.h"
#include "../serialization/serializable.h"
#include "../serialization/serializable_value.h"
#include "../serialization/unique_serializable.h"

class GameState : public unique_serializable {
private:
    CurrentStack _stack;
    DrawPile _draw;
    std::vector<CardCollection>  _cards;
    std::vector<Player> _players;
    std::vector<Team> _teams;
    serializable_value<int>* _previousPlayerIdx;
    serializable_value<int>* _currentPlayerIdx;
    serializable_value<std::string>* _winningPlayerId;
    serializable_value<std::string>* _dragonTargetPlayerId;
    serializable_value<int>* _turnCounter;
    serializable_value<int>* _roundCounter;
    serializable_value<std::string>* _gamephase; 
    
    // from_diff constructor
    GameState(std::string id);

    // deserialization constructor
    GameState(
            std::string id,
            CurrentStack _stack,
            DrawPile _draw,
            std::vector<CardCollection> _cards,
            std::vector<Player> _players,
            std::vector<Team> _teams,
            serializable_value<int>* _previousPlayerIdx,
            serializable_value<int>* _currentPlayerIdx,
            serializable_value<std::string>* _winningPlayerId,
            serializable_value<std::string>* _dragonTargetPlayerId,
            serializable_value<int>* _turnCounter,
            serializable_value<int>* _roundCounter,
            serializable_value<std::string>* _gamephase
            );

// State Changes
    void turn(); // Changes the index that points to the current player
    void order_player_vector(); // Orders _players vector to have players in alternating teams next to each other

// Accessors
    bool is_current_player(int index); // Checks if specified player is the current player
    int get_player_index(std::string player_id); // Returns the index pointing to the specified player
    std::string round_is_finished(); // Returns either the teamname of the team that won, that no team won, or that the round isn't finished yet
    bool wrap_up_round(); // checks if the round is finished and then wraps it up according to the Tichu ruleset
    bool wrap_up_play(); // check if the current play is finished and then wraps it up according to the Tichu ruleset

public:

// Public Constructor
    GameState();

// Deconstructor
    ~GameState();

// Accessors
    serializable_value<std::string>* get_gamephase(); // Returns the current gamephase (team_view, switch_view, game_view, end_round_view, end_game_view)
    serializable_value<int>* get_current_player_index(); // Returns the current player index
    int get_number_of_players(); // Returns the number of players in this game
    std::vector<Player> get_players(); // Returns the players in this game in play order
    std::vector<Player> get_players_ordered(Player player); // Returns the players in this game in play order, with the specified player beginning
    std::vector<Team> get_teams(); // Returns the teams
    bool is_current_player(Player player); // Checks if specified player is the current player
    int get_player_index(Player player); // Returns the index pointing to the specified player

    serializable_value<int>* get_round_counter(); // Returns the round counter

    std::vector<Card> get_cards(); // Returns the cards on top of the stack
    int get_stack_points(); // Returns the points in the stack

// State Change
    bool play(Player player, CardCollection cards, std::string& err); // play_cards_request Let specified player play his requested cards
    bool play_dragon(Player player, CardCollection cards, Player other_player, std::string& err); // play_dragon_request Let specified player play his requested cards and memorize other player
    bool pass(Player player, std::string& err); // pass_request, If a player doesn't want to play, he can just pass (pass_request)
    bool bomb(Player player, CardCollection cards, std::string& err); // bomb_reuest, If a player wants to play a bomb, let him to do it
    bool tichu(Player player, std::string type, std::string& err); // tichu_request, Let player announce tichus
    bool add_player_to_game(Player player); // join_game_request, Let's a player join the game
    bool remove_player_from_game(Player player); // exit_game_request, Let's a player leave the game
    bool add_player_to_team(Player player, std::string team); // join_team_request, Let's a player join a team
    bool swap_cards(Player player, CardCollection cards, std::string& err); // swap_cards_request, Let's player exchange cards

    bool start_game(); // start_game_request, Let's players start a game
    bool start_round(); // start_round_request, Let's players start a round
    // resets the game when a player is removed
    bool reset_game(std::string& err);
// Serialization & Deserialization
    static GameState from_json(const rapidjson::Value& json);
    bool swap_players(Player to_add, Player to_remove, std::string&);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};


#endif //TICHU_GAMESTATE_H