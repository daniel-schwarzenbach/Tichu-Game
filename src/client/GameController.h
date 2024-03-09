#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "widgets/ConnectionView.h"
#include "widgets/GameView.h"
#include "widgets/CardSwapView.h"
#include "widgets/TeamView.h"
#include "widgets/EndRoundView.h"
#include "widgets/EndGameView.h"
#include "network/ResponseListenerThread.h"
#include "../common/game_state/GameState.h"

class GameController {

public:
    static void init();
    static void init(std::string const& playerName); 

    static void connectToServer();
    static void updateGameState(GameState newGameState);
    static void updateViews();
    static void playCards();
    static void award_cards(CardCollection giftCard,std::string target_player_id);
    static void bomb();
    static void exitGame();
    static void getCards(int nof_cards);
    static void joinTeam(std::string team);
    static void newGame();
    static void pass();
    static void startGame();
    static void startRound();
    static void swapCards(CardCollection card_collection);
    static void tichuDeclaration(std::string tichuType);
    static CardCollection* _cardSelection;
    static CardCollection* _singleSelection;
    static GameState* getCurrentGameState();
    static Player* getMe();
    

    static void showError(const std::string& title, const std::string& message);
    static void showStatus(const std::string& message);
    static void showNewRoundMessage(GameState* oldGameState, GameState* newGameState);
    static void showGameOverMessage();
    static void dragon();
    static void updateSelectedCards();

private:
    static Player* showDragonPopUp(Player* player1, Player* player2);
    static ConnectionView* _ConnectionView;
    static GameView* _GameView;
    static CardSwapView* _CardSwapView;
    static EndRoundView* _EndRoundView;
    static EndGameView* _EndGameView;
    static TeamView* _TeamView;
    static Player* _me;
    static GameState* _currentGameState;
};

#endif // GAMECONTROLLER_H
