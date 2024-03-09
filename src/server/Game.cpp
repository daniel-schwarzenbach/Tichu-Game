//
// Created by Daniel on Spring 2023 inspired by Manuel
//
// The Game class is a wrapper around the GameState of an active 
// instance of the game.
// This class contains functions to modify the contained GameState.

#include "Game.hpp"

#include "Server.hpp"
#include "../common/network/responses/game_state_response.h"



Game::Game() {
    _gameState = new GameState();
    _has_started = false;
    _has_finished = false;
}

GameState* Game::get_game_state() {
    return _gameState;
}

std::string Game::get_id() const{
    return _gameState->get_id();
}

bool Game::play(Player player, 
                CardCollection cards, 
                std::string& err){
    _mutex.lock();
    if(_gameState->play(player, cards, err)){
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    _mutex.unlock();
    return false;
}

bool Game::is_full(){
    if(_gameState->get_number_of_players() >= 4)
        return true;
    else
        return false;
}

bool Game::is_empty(){
    if(_gameState->get_number_of_players() == 0){
        return true;
    }
    else{
        return false;
    }
}

bool Game::is_started(){
    return _has_started;
}

bool Game::is_finished(){
    return _has_finished;
}


bool Game::pass(Player player,
                std::string& err){
    _mutex.lock();
    if(_gameState->pass(player, err)){
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    _mutex.unlock();
    return false;
}

bool Game::add_player_to_game(  Player player,
                                string& err){
    _mutex.lock();
    if(_gameState->add_player_to_game(player)){
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    err = "failed to add player to game";
    _mutex.unlock();
    return false;
}
bool Game::swap_players(Player& to_add, Player& to_remove, string& err
){
    //_mutex.lock();
    if(_gameState->swap_players(to_add, to_remove, err)){
        to_add.set_gameID(to_remove.get_gameID());
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    to_add);
        //_mutex.unlock();
        return true;
    }
    err = "failed to add player to game";
    //_mutex.unlock();
    return false;
}

bool Game::remove_player_from_game( Player player,
                                    string& err){
    _mutex.lock();
    if(_gameState->remove_player_from_game(player)){
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    err = "failed to remove player from game";
    _mutex.unlock();
    return false;
}

bool Game::start_game(Player player, string& err){
    _mutex.lock();
    if(_gameState->start_game()){
        _has_started = true;
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    err = "failed to start game GameSate function";
    _mutex.unlock();
    return false;
}

bool Game::start_round(Player player, string& err){
    _mutex.lock();
    if(_gameState->start_round()){
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    err = "failed to start round GameState function";
    _mutex.unlock();
    return false;
}

bool Game::setup_round(Player player,
                       string& err){
    cout << ERROR << "setup_round(Player,string&) is not implemented"
         << endl;
    err = "setup_round(Player,string&) is not implemented";
    return false;
}

bool Game::swap_cards(Player player, CardCollection cards, 
                      string& err){
    _mutex.lock();
    if(_gameState->swap_cards(player, cards, err)) {
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    _mutex.unlock();
    return false;
}
    

bool Game::bomb(Player player, CardCollection cards ,string& err){
    _mutex.lock();
    if(_gameState->bomb(player, cards, err)){
        game_state_response msg 
                = game_state_response(this->get_id(), *_gameState);
        Server::broadcast_message(  msg, 
                                    _gameState->get_players(), 
                                    player);
        _mutex.unlock();
        return true;
    }
    _mutex.unlock();
    return false;
}

bool Game::play_dragon(  Player player, CardCollection cards, 
                    Player other_player, 
                    std::string& err){
    _mutex.lock();
    if(!_gameState->play_dragon(player, cards, other_player, err)){
        _mutex.unlock();
        return false;
    }
    game_state_response msg 
            = game_state_response(this->get_id(), *_gameState);
    Server::broadcast_message(  msg, 
                                _gameState->get_players(), 
                                player);
    _mutex.unlock();
    return true;
}

bool Game::tichu(Player player, std::string type, string& err){
    _mutex.lock();
    if(!_gameState->tichu(player, type, err)) {
        _mutex.unlock();
        return false;
    }
    game_state_response msg 
            = game_state_response(this->get_id(), *_gameState);
    Server::broadcast_message(  msg, 
                                _gameState->get_players(), 
                                player);
    _mutex.unlock();
    return true;
}

bool Game::add_player_to_team(Player player,string team,string& err){
    _mutex.lock();
    if(!_gameState->add_player_to_team(player, team)){
        err = "team is already full!";
        _mutex.unlock();
        return false;
    }
    game_state_response msg 
            = game_state_response(this->get_id(), *_gameState);
    Server::broadcast_message(  msg, 
                                _gameState->get_players(), 
                                player);
    _mutex.unlock();
    return true;
}

bool Game::operator==(Game const& otherGame) const {
    return this->get_id() == otherGame.get_id();
}

bool Game::reset_game(Player player, string& err){
    _mutex.lock();
    if(!_gameState->reset_game(err)) {
        _mutex.unlock();
        return false;
    }
    _has_started = false;
    game_state_response msg
            = game_state_response(this->get_id(), *_gameState);
    Server::broadcast_message(  msg,
                                _gameState->get_players(),
                                player);
    _mutex.unlock();
    return true;
}