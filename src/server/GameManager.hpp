//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The GameManager only exists on the server side. It stores 
// all currently active games and offers functionality to retrieve 
// game instances by id and adding players to games.
// If a new player requests to join a game but no valid Game is 
// available, then this class will generate a new Game and add it to 
// the unordered_map of (active) game instances.

#ifndef TICHU_GAME_MANAGER_H
#define TICHU_GAME_MANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "Game.hpp"

class GameManager {

private:

    inline static std::shared_mutex _mutex;
    // gameId -> game
    static std::unordered_map<std::string, Game*> _idToGame;
    // playerId -> player_name
    static std::unordered_map<std::string, std::string> _disconnects;

    
    static Game* find_joinable_game_instance(Player& player);
    static bool name_in_disconnects(string const& name);
    
public:
    static bool report_disconnect(const std::string& playerId);
    static bool id_in_disconnects(string const& playerId);
    static Game* create_new_game();
    // returns true if the desired Game 'gameId' was found or 
    // false otherwise.
    // The found game instance is written into game.
    static bool get_game_from_id(
            const std::string& gameId, 
            Game*& game);
    // returns true if the desired player 'playerId' was found and 
    // is connected to a Game.
    // The found player and Game will be written into 'player' 
    // and 'game'
    static bool get_player_and_game_from_id(
            const std::string& playerId, Player*& player, 
            Game*& game, std::string& err);

    // Try to add 'player' to any game. Returns true if 'player' is 
    // successfully added to a Game.
    // The joined Game will be written into 'game'.
    static bool add_player_to_any_game(
            Player* player, 
            Game*& game, 
            std::string& err);
    // Try to add 'player' to the provided 'game'. Returns true if 
    // success and false otherwise.
    static bool add_player_to_game( Player* player, 
                                Game*& game, 
                                std::string& err);


    static bool remove_player_from_game(  Player* player, 
                                    const std::string& gameId, 
                                    string& err);

    static bool remove_player_from_game(  Player* player, 
                                    Game*& game, 
                                    string& err);

};


#endif //TICHU_GAME_MANAGER_H
