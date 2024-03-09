//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The Server handles all incoming messages and offers 
// functionality to broadcast messages
// to all connected players of a game.

#ifndef TICHU_SERVER_H
#define TICHU_SERVER_H

#include <thread>
#include <functional>
#include <unordered_map>
#include <shared_mutex>

#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_acceptor.h"

#include "../common/network/requests/client_request.h"
#include "../common/network/responses/server_response.h"
#include "../common/game_state/Player.h"
#include "../common/game_state/GameState.h"

class Server {
private:

    inline static Server* _instance;
    inline static std::shared_mutex _mutex;
    inline static sockpp::tcp_acceptor _acceptor;

    inline static std::unordered_map<std::string, 
                                     std::string> 
            _playerIdToAddress;
    
    inline static std::unordered_map<std::string, 
                                     sockpp::tcp_socket> 
            _addressToSocket;

    void connect(const std::string& url, const uint16_t  port);

    static void listener_loop();
    static void read_message(
            sockpp::tcp_socket socket,
            const std::function<void(
                    const std::string&, 
                    const sockpp::tcp_socket::addr_t&
            )>& message_handler
    );

    static void handle_incoming_message(
            const std::string& msg, 
            const sockpp::tcp_socket::addr_t& peerAddress
    );
    static ssize_t send_message(const std::string& msg,
                                const std::string& address);

public:
    Server();
    ~Server();

    // Used to broadcast a server_response 
    // (e.g. a full_state_response) to all 'players' except 'exclude'
    static void broadcast_message(
            server_response& msg, 
            const std::vector<Player>& players, 
            const Player exclude
    );

    static void kick_player(std::string player_id);
};



#endif //TICHU_SERVER_H
