//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The PlayerManager only exists on the server side. It stores all 
// connected users since starting the server. It offers
// functionality to retrieve players by id or adding players when 
// they first connect to the server.
//

#include "PlayerManager.hpp"

// Initialize static map
std::unordered_map< string, 
                    Player*> PlayerManager::_idToPlayer = {};

bool PlayerManager::get_player(const string& playerId, 
                               Player *&playerPtr){
    playerPtr = nullptr;
    _mutex.lock_shared();
    auto it = PlayerManager::_idToPlayer.find(playerId);
    if (it != _idToPlayer.end()) {
        playerPtr = it->second;
    }
    _mutex.unlock_shared();
    return playerPtr;
}

bool PlayerManager::add_or_get_player(
        string name, const string& playerId, 
        Player *&playerPtr 
){
    if (get_player(playerId, playerPtr)) {
        return true;
    }
    playerPtr = new Player(playerId, name);
    _mutex.lock();    // exclusive
    PlayerManager::_idToPlayer.insert({playerId, playerPtr});
    _mutex.unlock();
    return true;
}

bool PlayerManager::remove_player(const string& playerId, 
                                  Player *&player){
    if (get_player(playerId, player)) {
        _mutex.lock();    // exclusive
        int nof_removals = PlayerManager::_idToPlayer.erase(playerId);
        _mutex.unlock();
        return true;
    }
    return false;
}

