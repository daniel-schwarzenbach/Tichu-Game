//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The GameManager only exists on the server side. It stores all 
// currently active games and offers functionality to retrieve game 
// instances by id and adding players to games.
// If a new player requests to join a game but no valid Game is 
// available, then this class will generate a new Game and add it to 
// the unordered_map of (active) games.

#include "GameManager.hpp"

#include "PlayerManager.hpp"
#include "Server.hpp"

bool GameManager::id_in_disconnects(string const& id){
    for(auto disconnect : _disconnects){
        if(disconnect.first == id){
            return true;
        }
    }
    return false;
}

bool GameManager::name_in_disconnects(string const& name){
    for(auto disconnect : _disconnects){
        if(disconnect.second == name){
            return true;
        }
    }
    return false;
}

// Initialize static maps
std::unordered_map<string, Game*> GameManager::_idToGame = {};
std::unordered_map<string, string> GameManager::_disconnects = {};

Game *GameManager::find_joinable_game_instance(Player& player) {
    std::vector<string> to_remove;
    Game* res = nullptr;
    _mutex.lock_shared();
    // check disconnects
    if(name_in_disconnects(player.get_name())){
        string otherId = "";
        // find the id
        for(auto disconnect : _disconnects){
            if(player.get_name() == disconnect.second){
                otherId = disconnect.first;
                break;
            }
        }
        if(otherId == ""){
            cout << ERROR << "empty Id!" << endl;
        }
        Player* other;
        Game* game;
        std::string err;
        if(get_player_and_game_from_id(otherId,other,game,err)){
            // swap places
            string err2;
            if(game->swap_players(player, *other, err2)){
                _disconnects.erase(otherId);
                PlayerManager::remove_player(otherId, other);
                cout << "player " << player.get_id() 
                     << " plays now for " << player.get_id();
                return game;
            }
            else{
                cout << err << endl;
            }
        }
        else{
            cout << "weird disconnect!?!?? " << err << endl;
        }
    }
    // check every game
    for (auto it = _idToGame.begin(); it != _idToGame.end(); it++) {
        // check if there are any finished games that can be removed
        if (    it->second->is_finished() ||
                it->second->is_empty()) {    
            to_remove.push_back(it->first);
        }
        else if (!it->second->is_full() && 
                   !it->second->is_started()
        ){
            res = it->second; // found a non-full, non-started game
            break;
        }
    }
    _mutex.unlock_shared();

    if (res == nullptr) {
        // couldn't find a non-full, 
        // non-started game -> create a new one
        res = create_new_game();
    }

    // remove all finished games
    if (to_remove.size() > 0) {
        _mutex.lock();
        for (auto& gameId : to_remove) {
            _idToGame.erase(gameId);
        }
        _mutex.unlock();
    }
    return res;
}

Game* GameManager::create_new_game() {
    Game* newGame = new Game();
    _mutex.lock();  // exclusive
    GameManager::_idToGame.insert({newGame->get_id(), newGame});
    _mutex.unlock();
    return newGame;
}


bool GameManager::get_game_from_id(const string& gameId, 
                                   Game *&game) {
    game = nullptr;
    _mutex.lock_shared();
    auto it = GameManager::_idToGame.find(gameId);
    if (it != _idToGame.end()) {
        game = it->second;
    }
    _mutex.unlock_shared();
    return game != nullptr;
}

bool GameManager::get_player_and_game_from_id(
        const string& player_id, 
        Player *&player, Game *&game, 
        string& err) 
{
    if (PlayerManager::get_player(player_id, player)) {
        if (GameManager::get_game_from_id(
                    player->get_gameID(), 
                    game)
        ){
            return true;
        }else{
            err = "Could not find gameId" + player->get_gameID() + 
                  " associated with this player";
        }
    }else{
        err = "Could not find requested player " + player_id + 
              " in database.";
    }
    return false;
}


bool GameManager::add_player_to_any_game(Player *player, 
                                            Game*& game, 
                                            string& err){

    // check that player is not already subscribed to another game
    if (player->get_gameID() != "") {
        if (    game != nullptr && 
                player->get_gameID() != game->get_id()) {
            err =   "Could not join game with id " + 
                    game->get_id() + 
". Player is already active in a different game with id " 
                    + player->get_gameID();
        } else {
            err = 
"Could not join game. Player is already active in a game";
        }
        return false;
    }

    if (game == nullptr) {
        // Join any non-full, non-started game
        for (int i = 0; i < 10; i++) {
            // make at most 10 attempts of joining a src (due to 
            // concurrency, the game could already be full or started 
            // by the time try_add_player_to_any_game() is invoked) 
            // But with only few concurrent requests it should 
            // succeed in the first iteration.
            game = find_joinable_game_instance(*player);
            if (add_player_to_game(player, game, err)) {
                return true;
            }
        }
        return false;
    }
    else {
        return add_player_to_game(player, game, err);
    }
}


bool GameManager::add_player_to_game(Player *player, 
                                Game *&game,
                                string& err) {
    if (player->get_gameID() != "") {
        if (player->get_gameID() != game->get_id()){
            err = 
"Player is already active in a different src with id " 
                        + player->get_gameID();
            return false;
        }else{
            err = "Player is already active in this src";
            return true;
        }
        
    }
    
    if (game->add_player_to_game(*player, err)) {
        // mark that this player is playing in a src
        player->set_gameID(game->get_id());   
        return true;
    } else {
        return false;
    }
}

bool GameManager::remove_player_from_game(Player *player, 
                                          const string& gameId, 
                                          string &err){
    Game* game = nullptr;
    if (get_game_from_id(gameId, game)) {
        return remove_player_from_game(player, game, err);
    } else {
        err = 
"The requested src could not be found. Requested src id was " 
                + gameId;
        return false;
    }
}

bool GameManager::remove_player_from_game(Player *player, 
                                          Game *&game, 
                                          string &err){
    if(game->remove_player_from_game(*player, err)){
        // erase the disconnect
        if(name_in_disconnects(player->get_name())){
            if(_disconnects.find(player->get_id())->second ==
                    player->get_name())
            {
                _disconnects.erase(player->get_id());
            }
        }
        if(game->is_empty()){
            _idToGame.erase(game->get_id());
        }
        return true;
    }
    // else
    return false;
}

bool GameManager::report_disconnect(const std::string& playerId){
    Player* player;
    Game* game;
    string err;
    if(get_player_and_game_from_id(playerId,player,game, err)){
        _disconnects.emplace(playerId, player->get_name());
        return true;
    } else{
        return false;
    }
}



