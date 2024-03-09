//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The Game class is a wrapper around the game_state of an active 
// instance of the game.
// This class contains functions to modify the contained game_state.

#ifndef TICHU_GAME_H
#define TICHU_GAME_H

#include <vector>
#include <string>
#include <mutex>

#include "dschwarze.hpp"

#include "../common/game_state/Player.h"
#include "../common/game_state/GameState.h"

class Game {

private:
    GameState* _gameState;
    bool _has_started = false;
    bool _has_finished = false;
    bool is_player_allowed_to_play(Player player);
    inline static std::mutex _mutex;

public:
    Game();
    ~Game() {
        if (_gameState != nullptr) {
            delete _gameState;
        }
        _gameState = nullptr;
    }
    std::string get_id() const;

    GameState* get_game_state();

    bool is_full();
    bool is_started();
    bool is_finished();
    bool is_empty();

    bool reset_game(Player player, string& err);
    
    bool play(Player player, CardCollection cards, std::string& err);
    bool pass(Player player, string& err);
    bool add_player_to_game(Player player, string& err);
    // no mutex
    bool swap_players(Player& to_add, Player& to_remove, string& err);

    
    #ifndef ARSCHLOECHLE
    bool bomb(Player player, CardCollection cards, std::string& err);
    bool play_dragon(Player player, CardCollection cards, 
                Player other_player, 
                std::string& err);
    bool tichu(Player player, std::string type, string& err);
    bool swap_cards(Player player, CardCollection cards, string& err);
    #endif

    bool remove_player_from_game(Player player, string& err);
    bool add_player_to_team(Player player, string team, string& err);



    bool start_round(Player player, string& err);
    bool start_game(Player excludePlayer, string& err);
    bool setup_round(Player player, string& err);

    bool operator==(Game const& otherGame) const;
};


#endif //TICHU_GAME_H

