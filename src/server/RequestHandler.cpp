//
// Created by Daniel on Spring 2023 inspired by Manuel
//

#ifndef TICHU_REQUEST_HANDLER_CPP
#define TICHU_REQUEST_HANDLER_CPP

// server
#include "RequestHandler.hpp"
#include "PlayerManager.hpp"
#include "GameManager.hpp"
#include "Game.hpp"
#include "AllRequests.hpp"


request_response* RequestHandler::handle_request(
        const client_request* const req) 
{

    // Prepare variables that are used by every request type
    Player* player;
    std::string err;
    Game* gamePtr = nullptr;

    // Get common properties of requests
    RequestType type = req->get_type();
    std::string req_id = req->get_req_id();
    std::string game_id = req->get_game_id();
    std::string player_id = req->get_player_id();


    // Switch behavior according to request type
    switch(type) {
        // ##################### award_cards #####################  //

        // Do we actually need this request? (by Marc)

        /* case RequestType::award_cards:{
            if (!GameManager::get_player_and_game_from_id(
                        player_id, 
                        player, 
                        gamePtr, 
                        err)
            ) 
                return new request_response(
                        "",req_id,false,nullptr,err
                );

            award_cards_request* award_req = 
                    ((award_cards_request*) req);
            string otherPlayerId = award_req->get_target_player();
            Player* otherPlayer;
            Game* otherGamePtr;

            if(!GameManager::get_player_and_game_from_id(
                    otherPlayerId, otherPlayer, otherGamePtr, err)
            )
                return new request_response(
                        "", req_id, false, nullptr, err);

            if(!(*otherGamePtr == *gamePtr)){
                err = 
"target player in the award_cards_request is not in the same Game!";
                return new request_response(
                        "", req_id, false, nullptr, err);
            }
            // get points
            rapidjson::Value* jsonValue 
                    = award_req->get_card_collection();
            CardCollection cards
                    = CardCollection::from_json(*jsonValue);
            CurrentStack stack;
            stack.change_points(cards);
            serializable_value<int> points(stack.get_points());

            if (gamePtr->play_dragon(*player,cards,*otherPlayer,err)){
                return new request_response(
                        gamePtr->get_id(), req_id, true,
                        gamePtr->get_game_state()->to_json(), err);
            }
            //else
            return new request_response("",req_id,false,nullptr,err); 
        }*/
        // ##################### bomb ############################  //
        case RequestType::bomb:{
            if (GameManager::get_player_and_game_from_id(
                        player_id,player,gamePtr,err)
                ) {
                CardCollection cards;
                bomb_request* bomb_req = ((bomb_request*) req);
                rapidjson::Value* jsonValue = bomb_req->get_cards();
                cards = CardCollection::from_json(*jsonValue);

                if (gamePtr->bomb(*player, cards, err)) {
                    return new request_response(
                            gamePtr->get_id(), req_id, true,
                            gamePtr->get_game_state()->to_json(),err);
                }
            }
            return new request_response(
                    "", req_id, false, nullptr, err);
        }
        // ##################### exit_game #######################  //
        case RequestType::exit_game:{
            if(     !GameManager::get_player_and_game_from_id(
                            player_id, player, gamePtr, err)
            ){
                return new request_response(
                        "", req_id, false, nullptr, err);
            }
            if(!gamePtr->remove_player_from_game(*player, err)){
                return new request_response(
                        "", req_id, false, nullptr, err);
            }
            if(     !GameManager::remove_player_from_game(
                            player, gamePtr,err)
            ){
                return new request_response(
                        "", req_id, false, nullptr, err);
            }
            return new request_response(
                    gamePtr->get_id(), req_id, true,
                    gamePtr->get_game_state()->to_json(), err);
        }
        // ##################### get_cards #######################  //
        case RequestType::get_cards:{
            cout    << WARNING 
                    << "request get_cards is not implemented :("
                    << endl;
            err = "request get_cards is not implemented :(";
            return new request_response("",req_id,false,nullptr,err);
        }
        // ##################### join_game #######################  //
        case RequestType::join_game:{
            string player_name = 
                    ((join_game_request*)req)->get_player_name();
            // Create new player or get existing one with that name
            PlayerManager::add_or_get_player(player_name, 
                                            player_id, 
                                            player);

            if (game_id.empty()) {
                // join any game
                if (GameManager::add_player_to_any_game(
                            player, gamePtr, err)
                ){
                    // gamePtr got updated to the joined game

                    // return response with full game_state attached
                    return new request_response(
                            gamePtr->get_id(), req_id, true,
                            gamePtr->get_game_state()->to_json(),err);
                } else {
                    // failed to find game to join
                    return new request_response(
                            "", req_id, false, nullptr, err);
                }
            } else {
                // join a specific game denoted by req->get_game_id()
                if (GameManager::get_game_from_id(game_id, gamePtr)){
                    if (GameManager::add_player_to_game(
                                player, gamePtr, err)
                    ){
                        // return response with full game_state 
                        // attached
                        return new request_response(
                                game_id, req_id, true,
                                gamePtr->get_game_state()->to_json(), 
                                err);
                    } else {
                        // failed to join requested game
                        return new request_response(
                                "", req_id, false, nullptr, err);
                    }
                } else {
                    // failed to find requested game
                    return new request_response(
                                "", req_id, false, nullptr, 
                                "Requested game could not be found.");
                }
            }
        }
        // ##################### new_game ########################  //
        case RequestType::new_game:{
            // join any game
                gamePtr = GameManager::create_new_game();
                if (GameManager::add_player_to_game(
                            player, gamePtr, err)
                ){
                    // gamePtr got updated to the joined game

                    // return response with full game_state attached
                    return new request_response(
                            gamePtr->get_id(), req_id, true,
                            gamePtr->get_game_state()->to_json(),err);
                } else {
                    // failed to find game to join
                    return new request_response(
                            "", req_id, false, nullptr, err);
                }
        }
        // ##################### pass ############################  //
        case RequestType::pass:{
            if(GameManager::get_player_and_game_from_id(
                        player_id, player, gamePtr, err)
            ){
                if (gamePtr->pass(*player, err)) {
                    return new request_response(
                            gamePtr->get_id(), req_id, true,
                            gamePtr->get_game_state()->to_json(),err);
                }
            }
            return new request_response(
                    "", req_id, false, nullptr, err);
        }
        // ##################### play_cards ######################  //
        case RequestType::play_cards:{
            if (GameManager::get_player_and_game_from_id(
                        player_id, player, gamePtr, err)
            ) {
                CardCollection cards;
                play_cards_request* play_req = 
                        ((play_cards_request*) req);
                rapidjson::Value* jsonValue = play_req->get_cards();
                cards = CardCollection::from_json(*jsonValue);
                #ifdef SERVER_DEBUG
                if(cards.get_cards().size() == 0)
                    cout << WARNING << "no cards found!" << endl;
                #endif

                if (gamePtr->play(*player, cards, err)) {
                    return new request_response(
                            gamePtr->get_id(), req_id, true,
                            gamePtr->get_game_state()->to_json(),err);
                }
            }

            return new request_response("",req_id,false,nullptr,err);
        }
        // ##################### start_game ######################  //
        case RequestType::start_game:{
           if (GameManager::get_player_and_game_from_id(player_id, 
                                                        player, 
                                                        gamePtr, 
                                                        err)) {
                if (gamePtr->start_game(*player, err)) {
                    return new request_response(
                            gamePtr->get_id(), req_id, true,
                            gamePtr->get_game_state()->to_json(),err);
                }
            }
            return new request_response("",req_id,false,nullptr,err);
        }
        // ##################### start_roud ######################  //
        case RequestType::start_round:{
            if (!GameManager::get_player_and_game_from_id(
                    player_id, player, gamePtr, err)
            )
                return new request_response("", req_id, false, 
                                            nullptr, err);

            if (!gamePtr->start_round(*player, err))
                return new request_response("", req_id, false, 
                                            nullptr, err);
            
            return new request_response(
                    gamePtr->get_id(), req_id, true,
                    gamePtr->get_game_state()->to_json(), err);
        }
        // ##################### swap_cards ######################  //
        case RequestType::swap_cards:{
            if (!GameManager::get_player_and_game_from_id(
                    player_id, player, gamePtr, err)
            )
                return new request_response("", req_id, false, 
                                            nullptr, err);
            // Get Cards
            swap_cards_request* swap_req = ((swap_cards_request*)req);
            rapidjson::Value* jsonValue = swap_req->get_cards();
            CardCollection cards = 
                    CardCollection::from_json(*jsonValue);
            if (!gamePtr->swap_cards(*player, cards, err))
                return new request_response("", req_id, false, 
                                            nullptr, err);
            // else (swap_cards is successfull)
            #ifdef SERVER_DEBUG
            cout << INFO << "swap_card was succesfull!" << endl;
            #endif
            return new request_response(
                    gamePtr->get_id(), req_id, true,
                    gamePtr->get_game_state()->to_json(), err);
        }
        // ##################### tichu ###########################  //
        case RequestType::tichu:{
            if (!GameManager::get_player_and_game_from_id(
                    player_id, player, gamePtr, err)
            )
                return new request_response("", req_id, false, 
                                            nullptr, err);
            // get Tichu Type 
            tichu_request* tichu_req = ((tichu_request*)req);
            string tichuType = tichu_req->get_tichu();
            if (!gamePtr->tichu(*player, tichuType, err))
                return new request_response("", req_id, false, 
                                            nullptr, err);
            
            return new request_response(
                    gamePtr->get_id(), req_id, true,
                    gamePtr->get_game_state()->to_json(), err);
        }

        // ##################### play_dagon ######################  //
        case RequestType::play_dragon:{
            if (!GameManager::get_player_and_game_from_id(
                    player_id, player, gamePtr, err)
            )
                return new request_response("", req_id, false, 
                                            nullptr, err);
            // get other Player 
            play_dragon_request* dragon_req = 
                    ((play_dragon_request*)req);
            string otherPlayerId = dragon_req->get_otherPlayerID();
            Player* otherPlayer;
            Game* otherGamePtr;

            if(!GameManager::get_player_and_game_from_id(
                    otherPlayerId, otherPlayer, otherGamePtr, err)
            )
                return new request_response(
                        "", req_id, false, nullptr, err);

            if(!(*otherGamePtr == *gamePtr)){
                err = 
"target player in the award_cards_request is not in the same Game!";
                return new request_response(
                        "", req_id, false, nullptr, err);
            }
            
            // get cards
            rapidjson::Value* jsonValue = dragon_req->get_cards();
            CardCollection cards =
                    CardCollection::from_json(*jsonValue);
            if (!gamePtr->play_dragon(*player, cards, 
                                      *otherPlayer, err))
                return new request_response("", req_id, false, 
                                            nullptr, err);
            
            return new request_response(
                    gamePtr->get_id(), req_id, true,
                    gamePtr->get_game_state()->to_json(), err);
        }

        // ##################### join_team #######################  //
        case RequestType::join_team:{
            if (!GameManager::get_player_and_game_from_id(
                    player_id, player, gamePtr, err)
            )
                return new request_response("", req_id, false, 
                                            nullptr, err);
            // get Team 
            join_team_request* team_req = ((join_team_request*)req);
            string team = team_req->get_team();
            
            if (!gamePtr->add_player_to_team(*player, team, err))
                return new request_response("", req_id, false, 
                                            nullptr, err);

            return new request_response(
                    gamePtr->get_id(), req_id, true,
                    gamePtr->get_game_state()->to_json(), err);
        }

        // ##################### UNKNOWN REQUEST ################## //
        default:
            return new request_response("", req_id, false, nullptr, 
                                        "Unknown RequestType");
    }
}

#endif //TICHU_REQUEST_HANDLER_CPP
