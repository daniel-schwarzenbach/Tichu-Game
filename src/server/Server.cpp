//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The server_network_manager handles all incoming messages and offers 
// functionality to broadcast messages
// to all connected players of a game.

#include "Server.hpp"
#include "RequestHandler.hpp"
#include "dschwarze.hpp"

// include server address configurations
#include "../common/network/default.conf"
#include "../common/network/responses/request_response.h"
#include "GameManager.hpp"
#include "timer.hpp"


static std::unordered_map<std::string, sockpp::tcp_socket> 
            _addressToSocket;



Server::Server() {
    if (_instance == nullptr) {
        _instance = this;
    }
    // Required to initialise sockpp
    sockpp::socket_initializer socketInitializer;
    // variables from "default.conf"
    this->connect(default_server_host, default_port); 
}

Server::~Server() = default;

void Server::connect(const std::string &url, const uint16_t port) {
    this->_acceptor = sockpp::tcp_acceptor(port);

    if (!_acceptor) {
        cerr << ERROR << "Error creating the acceptor: " 
             << _acceptor.last_error_str() 
             << endl;
        return;
    }

    cout << "Awaiting connections on port " << GREEN << port 
         << RESET << "..." 
         << endl;
    listener_loop();    // start endless loop
}

void Server::listener_loop() {
    // intentional endless loop
    while (true) {
        sockpp::inet_address peer;

        // Accept a new client connection
        sockpp::tcp_socket socket = _acceptor.accept(&peer);
        cout << "Received a connection request from " 
             << GREEN << peer << RESET << endl;

        if (!socket) {
            cerr << ERROR << "Error accepting incoming connection: "
                 << _acceptor.last_error_str() << std::endl;
        } else {
            _mutex.lock();
            
            _addressToSocket.emplace(
                    socket.peer_address().to_string(), 
                    std::move(socket.clone()));
            _mutex.unlock();

            // Create a listener thread and transfer the new stream to 
            // it. Incoming messages will be passed to 
            // handle_incoming_message().
            std::thread listener(
                    read_message,
                    std::move(socket),
                    handle_incoming_message);

            listener.detach();
        }
    }
}

// Runs in a thread and reads anything coming in on the 'socket'.
// Once a message is fully received, the string is passed on to the 
// 'handle_incoming_message()' function
void Server::read_message(
        sockpp::tcp_socket socket, 
        const std::function<void(
                const std::string&,
                const sockpp::tcp_socket::addr_t &
        )> &message_handler
) {
    // initializes socket framework underneath
    sockpp::socket_initializer sockInit;    

    char buffer[512]; // 512 bytes
    ssize_t count = 0;
    ssize_t msgBytesRead = 0;
    ssize_t msgLength = 0;
    string playerId;

    while ((count = socket.read(buffer, sizeof(buffer))) > 0) {
        try {
            int i = 0;
            std::stringstream streamMsgLength;
            while (buffer[i] != ':' && i < count) {
                streamMsgLength << buffer[i];
                i++;
            }
            msgLength = std::stoi(streamMsgLength.str());
            cout << "Expecting message of length " << msgLength 
                 << endl;

            // put everything after the message length declaration 
            // into a stringstream
            std::stringstream streamMsg;
            streamMsg.write(&buffer[i+1], count - (i + 1));
            msgBytesRead = count - (i + 1);

            // read the remaining packages
            while (msgBytesRead < msgLength && count > 0) {
                count = socket.read(buffer, sizeof(buffer));
                msgBytesRead += count;
                streamMsg.write(buffer, count);
            }

            if (msgBytesRead == msgLength) {
                // sanity check that really all bytes got read 
                // (possibility that count was <= 0, 
                // indicating a read error)
                std::string msg = streamMsg.str();
                // attempt to parse client_request from 'msg'
                // get playerID so we can kick him properly
                try{
                    rapidjson::Document jsonReq;
                    jsonReq.Parse(msg.c_str());
                    // try to parse a client_request from the json
                    client_request* req = 
                            client_request::from_json(jsonReq);
                    // check if this is a connection to a new player
                    playerId = req->get_player_id();
                } catch (const std::exception& e){
                    cout << ERROR << "could not parse msg to request!"
                         << "  Exeption is: " << e.what() << endl;
                }
                message_handler(msg, socket.peer_address());    
            } else {
                cerr << ERROR << 
"Could not read entire message. TCP stream ended before. Difference is " 
                     << msgLength - msgBytesRead << std::endl;
            }
        // Make sure the connection isn't torn down only because of 
        // a read error
        } catch (std::exception& e) { 
            std::cerr 
                    << ERROR << "while reading message from " 
                    << socket.peer_address() << std::endl 
                    << e.what() << std::endl;
        }
    }
    if (count <= 0) {
        cout << WARNING << "Read error [" << socket.last_error() 
             << "]: "
             << socket.last_error_str() << std::endl;
    }
    // try reconnecting player
    static const double reconnectTime = 0.0;
    #ifdef PRINT_NETWORK_MESSAGES
    cout << INFO 
         << "Player lost connection! He has "<<reconnectTime
         << "s to reconnect." 
         << " Otherwise the Server will reset the game!"<< endl;
    #endif
    Player* player;
    Game* game;
    string err;
    if(!GameManager::get_player_and_game_from_id(
            playerId,player,game,err)
    ){
        cout << endl << ERROR << "err" << endl;
    }
    _mutex.lock();
    #ifdef PRINT_NETWORK_MESSAGES
    cout << endl << "Time remaining: "  << endl;
    #endif
    Timer timer;
    timer.start();
    GameManager::report_disconnect(playerId);
    _mutex.unlock();
    while (timer.time() < reconnectTime)
    {
        _mutex.lock();
        if(!GameManager::id_in_disconnects(playerId)){
            #ifdef PRINT_NETWORK_MESSAGES
            cout << endl << "player has reconnected!" << endl << endl;
            #endif
            _mutex.unlock();
            break;
        }
        #ifdef PRINT_NETWORK_MESSAGES
        cout<< "\r" << (int)(reconnectTime - timer.time())
            << std::flush;
        #endif
        _mutex.unlock();
    }
    cout << endl << endl;
    // kick_player!
    if(GameManager::id_in_disconnects(playerId))
        kick_player(playerId);
    else{

    }
    cout << INFO << "Closing connection to " 
         << socket.peer_address() 
         << std::endl;
    socket.shutdown();
}


void Server::handle_incoming_message(
        const std::string& msg, 
        const sockpp::tcp_socket::addr_t& peer_address
){
    try {
        // try to parse a json from the 'msg'
        rapidjson::Document jsonReq;
        jsonReq.Parse(msg.c_str());
        // try to parse a client_request from the json
        client_request* req = client_request::from_json(jsonReq);

        // check if this is a connection to a new player
        std::string playerId = req->get_player_id();
        _mutex.lock_shared();
        if (    _playerIdToAddress.find(playerId) == 
                _playerIdToAddress.end()
        ){
            // save connection to this client
            _mutex.unlock_shared();
            cout << INFO << "New client with id " 
                 << playerId << endl;
            _mutex.lock();
            _playerIdToAddress.emplace( playerId, 
                                        peer_address.to_string());
            _mutex.unlock();
        } else {
            //_if(_lostPlayers.find(plyaerId))
            _mutex.unlock_shared();
        }
        #ifdef PRINT_NETWORK_MESSAGES
        cout << endl << GREEN << "Received valid request : " << RESET 
             << endl << msg << endl << endl;
        #endif
        // execute client request
        server_response* res = RequestHandler::handle_request(req);
        delete req;

        // transform response into a json
        rapidjson::Document* jsonRes = res->to_json();
        delete res;

        // transform json to string
        string resMsg = json_utils::to_string(jsonRes);

        #ifdef PRINT_NETWORK_MESSAGES
        cout << endl << GREEN << "Sending response : " << RESET 
             << endl << resMsg << endl << endl;
        #endif

        // send response back to client
        send_message(resMsg, peer_address.to_string());
        delete jsonRes;

    } catch (const std::exception& e) {
        cerr << WARNING
             << "Failed to execute client request. Content was :\n"
             << msg << std::endl
             << ERROR << e.what() << std::endl;
    }
}


void Server::kick_player(std::string player_id) {
    Player* player;
    Game* game;
    string err;
    #ifdef PRINT_NETWORK_MESSAGES
    cout << INFO << "attempt to kick player and restart the Game! " 
         << endl;
    #endif
    bool player_kicked = true;
    if(!GameManager::get_player_and_game_from_id(
            player_id,player,game,err)
    ){
        cout << ERROR << "Couln't get player to remove: "
             << err << endl;
        player_kicked = false;
    } else{
        // restart Game
        if(game->reset_game(*player, err)){
            #ifdef PRINT_NETWORK_MESSAGES
            cout << INFO <<  "Game restart has worked! "<< err<< endl;
            #endif
        }
        else{
            #ifdef PRINT_NETWORK_MESSAGES
            cout << WARNING <<  "Game restart hasn't worked!"<< endl;
            #endif
        }
        // remove player
        if(!GameManager::remove_player_from_game(player, game, err)){
            cout << ERROR << "Couln't remove player "
                << err << endl;
            player_kicked = false;
        }

    }
    if(player_kicked){
        #ifdef PRINT_NETWORK_MESSAGES
        cout << INFO << "Player got kicked properly" << endl;
        cout << INFO << "Restarting Game: "<< game->get_id() << endl;
        #endif
        
    }
    _mutex.lock();
    string address = _playerIdToAddress[player_id];
    _playerIdToAddress.erase(player_id);
    _addressToSocket.erase(address);
    _mutex.unlock();
}

ssize_t Server::send_message(const std::string &msg, 
                             const std::string& address){

    std::stringstream streamMsg;
    // prepend message length
    streamMsg << std::to_string(msg.size()) << ':' << msg; 
    return _addressToSocket.at(address).write(streamMsg.str());
}

void Server::broadcast_message(server_response& msg, 
                               const std::vector<Player>& players, 
                               const Player exclude){
    // write to JSON format
    rapidjson::Document* msgJson = msg.to_json();
    // convert to string  
    std::string msgString = json_utils::to_string(msgJson);   

#ifdef PRINT_NETWORK_MESSAGES
    cout << endl << GREEN << "Broadcasting message : " << RESET 
         << endl << msgString << endl << endl;
#endif

    _mutex.lock_shared();
    // send object_diff to all requested players
    try {
        for(auto& player : players) {
            if (player.get_id() != exclude.get_id()) {
                int nof_bytes_written = send_message(
                        msgString, 
                        _playerIdToAddress.at(player.get_id()));
            }
        }
    } catch (std::exception& exeption) {
        cerr << ERROR 
             << "Encountered error when sending state update: " 
             << exeption.what() << endl;
    }
    _mutex.unlock_shared();
    delete msgJson;
}



