//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The PlayerManager only exists on the server side. It stores all 
// connected users since starting the server. It offers functionality 
// to retrieve players by id or adding players when they first connect 
// to the server.
//

#ifndef TICHU_PLAYER_MANAGER_H
#define TICHU_PLAYER_MANAGER_H
// std::lib
#include <string>
#include <shared_mutex>
#include <unordered_map>
// common
#include "../common/game_state/Player.h"
// server
#include "dschwarze.hpp"
class PlayerManager {

private:
    inline static std::shared_mutex _mutex;
    static std::unordered_map<string, Player*> _idToPlayer;

public:
    static bool get_player(
            const string& playerId, 
            Player*& playerPtr
    );

    static bool add_or_get_player(
            string name, const string& playerId, 
            Player*& playerPtr
    );

    static bool remove_player(
            const string& playerId, 
            Player*& playerPtr
    );  
            // not implemented
};


#endif //TICHU_PLAYER_MANAGER_H
