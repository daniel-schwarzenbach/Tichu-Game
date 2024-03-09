#include <QThread>
#include <QMessageBox>
#include <QPushButton>

#include "GameController.h"
#include "../common/network/requests/join_game_request.h"
#include "../common/network/requests/start_game_request.h"
#include "../common/network/requests/play_cards_request.h"
#include "../common/network/requests/award_cards_request.h"
#include "../common/network/requests/bomb_request.h"
#include "../common/network/requests/exit_game_request.h"
#include "../common/network/requests/get_cards_request.h"
#include "../common/network/requests/join_team_request.h"
#include "../common/network/requests/new_game_request.h"
#include "../common/network/requests/pass_request.h"
#include "../common/network/requests/start_round_request.h"
#include "../common/network/requests/swap_cards_request.h"
#include "../common/network/requests/tichu_request.h"
#include "../common/network/requests/play_dragon_request.h"
#include "uiElements/CardCollectionView.h"


//#include "../common/network/requests/draw_card_request.h"
//#include "../common/network/requests/fold_request.h"
//#include "../common/network/requests/play_card_request.h"
#include "network/ClientNetworkManager.h"
#include <QDebug>
#include <QtCore/QMetaType>

Q_DECLARE_METATYPE(std::string)

ConnectionView* GameController::_ConnectionView = nullptr;
CardSwapView* GameController::_CardSwapView=nullptr;
GameView* GameController::_GameView = nullptr;
TeamView* GameController::_TeamView=nullptr;
EndRoundView* GameController::_EndRoundView=nullptr;
EndGameView* GameController::_EndGameView=nullptr;



Player* GameController::_me = nullptr;
GameState* GameController::_currentGameState = nullptr;

//card selection
CardCollection* GameController::_cardSelection =nullptr;

//single selection for swapping
CardCollection* GameController::_singleSelection =nullptr;

void GameController::init() {
    qRegisterMetaType<std::string>("std::string");
    //Creates all the necessary Views
    _cardSelection= new CardCollection();
    _singleSelection=new CardCollection();
    _ConnectionView = new ConnectionView();
    _GameView = new GameView();
    _EndRoundView = new EndRoundView();
    _EndGameView = new EndGameView();
    _TeamView = new TeamView();
    _CardSwapView= new CardSwapView();

    //Show Connection View on init
    _ConnectionView->show();
}

void GameController::init(std::string const& playerName) {
    qRegisterMetaType<std::string>("std::string");
    //Creates all the necessary Views
    _cardSelection= new CardCollection();
    _singleSelection=new CardCollection();
    _ConnectionView = new ConnectionView(playerName);
    _GameView = new GameView();
    _EndRoundView = new EndRoundView();
    _EndGameView = new EndGameView();
    _TeamView = new TeamView();
    _CardSwapView= new CardSwapView();

    //Show Connection View on init
    _ConnectionView->show();
}



void GameController::connectToServer(){

    QString inputServerAddress = GameController::_ConnectionView->getServerAddress().trimmed();
    QString inputServerPort = GameController::_ConnectionView->getServerPort().trimmed();
    QString inputPlayerName = GameController::_ConnectionView->getPlayerName().trimmed();

    // check that all values were provided
    if(inputServerAddress.isEmpty()) {
        GameController::showError("Input error", "Please provide the server's address");
        return;
    }
    if(inputServerPort.isEmpty()) {
        GameController::showError("Input error", "Please provide the server's port number");
        return;
    }
    if(inputPlayerName.isEmpty()) {
        GameController::showError("Input error", "Please enter your desired player name");
        return;
    }

    // convert host from QString to std::string
    std::string host = inputServerAddress.toStdString();

    // convert port from QString to uint16_t
    unsigned long portAsLong = inputServerPort.toULong();
    if(portAsLong > 65535) {
        GameController::showError("Connection error", "Invalid port");
        return;
    }

    uint16_t port = (uint16_t) portAsLong;

    // convert player name from QString to std::string
    std::string playerName = inputPlayerName.toStdString();

    // connect to network
    ClientNetworkManager::init(host, port);

    // send request to join game
    GameController::_me = new Player(playerName);
    join_game_request request = join_game_request(GameController::_me->get_id(), GameController::_me->get_name());
    ClientNetworkManager::sendRequest(request);
}


void GameController::updateGameState(GameState newGameState){

    // Make a copy of newGameState
    GameState* newStateCopy = new GameState(newGameState);

    // If _currentGameState already exists, delete it to avoid memory leaks
    if (_currentGameState != nullptr) {
        delete _currentGameState;
    }

    // Update _currentGameState to point to the new copy
    _currentGameState = newStateCopy;

    //Overwrite the player of the client
    std::vector<Player> players = _currentGameState->get_players_ordered(*_me);
    *_me = players[0];

    //Check whether connection to one player is lost
    if((_currentGameState->get_gamephase()->get_value() != "team_view") && players.size() != 4){
        GameController::showError("Connection Lost", "One Player lost the connection to the Server!");
    }else{
        updateViews();
    }
}

void GameController::updateViews(){
    if(_currentGameState->get_gamephase()->get_value() == "team_view"){
        _TeamView->buildGameState(_currentGameState, _me);

        //Close all other Views
        _GameView->close();
        _GameView->setIsShown(false);
        _EndRoundView->close();
        _EndRoundView->setIsShown(false);
        _EndGameView->close();
        _EndGameView->setIsShown(false);
        _ConnectionView->close();
        _CardSwapView->setIsShown(false);
        _CardSwapView->close();

        //Show TeamView
        if(_TeamView->isShown()){
            _TeamView->update();
        }else{
            _TeamView->setIsShown(true);
            _TeamView->show();
        }
    }else if (_currentGameState->get_gamephase()->get_value() == "switch_view"){
        _CardSwapView->buildGameState(_currentGameState, _me);

        //Close all other Views
        _TeamView->close();
        _TeamView->setIsShown(false);
        _EndRoundView->close();
        _EndRoundView->setIsShown(false);
        _EndGameView->close();
        _EndGameView->setIsShown(false);
        _ConnectionView->close();
        _GameView->close();
        _GameView->setIsShown(false);

        //Show GameView
        if(_CardSwapView->isShown()){
            _CardSwapView->update();
        }else{
            _cardSelection->remove_all_cards();
            _CardSwapView->setIsShown(true);
            _CardSwapView->show();
            _CardSwapView->resize(900,700);
        }
    
    }else if (_currentGameState->get_gamephase()->get_value() == "game_view"){
        _GameView->buildGameState(_currentGameState, _me);

        //Close all other Views
        _TeamView->close();
        _TeamView->setIsShown(false);
        _EndRoundView->close();
        _EndRoundView->setIsShown(false);
        _EndGameView->close();
        _EndGameView->setIsShown(false);
        _ConnectionView->close();
        _CardSwapView->setIsShown(false);
        _CardSwapView->close();

        //Show GameView
        if(_GameView->isShown()){
            _GameView->update();

            //Used to fix a UI Bug, where the selection was not visible
            if(_me->get_handcards().size() == _cardSelection->number_of_cards()){
                _cardSelection->remove_all_cards();
                _GameView->updateSelectedCards();
            }else{
                _GameView->updateSelectedCards();
            }
            
        }else{
            _cardSelection->remove_all_cards();
            _GameView->setIsShown(true);
            _GameView->show();
            _GameView->resize(1100,800);
        }
    }else if (_currentGameState->get_gamephase()->get_value() == "end_round_view"){
        _EndRoundView->updateGameState(_currentGameState, _me);

        //Close all other Views
        _TeamView->close();
        _TeamView->setIsShown(false);
        _GameView->close();
        _GameView->setIsShown(false);
        _EndGameView->close();
        _EndGameView->setIsShown(false);
        _ConnectionView->close();
        _CardSwapView->setIsShown(false);
        _CardSwapView->close();

        //Show _EndRoundView
        if(_EndRoundView->isShown()){
            _EndRoundView->update();
        }else{
            _cardSelection->remove_all_cards();
            _EndRoundView->setIsShown(true);
            _EndRoundView->show();
        }
    }else if (_currentGameState->get_gamephase()->get_value() == "end_game_view"){
        _EndGameView->updateGameState(_currentGameState, _me);

        //Close all other Views
        _TeamView->close();
        _TeamView->setIsShown(false);
        _GameView->close();
        _GameView->setIsShown(false);
        _EndRoundView->close();
        _EndRoundView->setIsShown(false);
        _ConnectionView->close();
        _CardSwapView->setIsShown(false);
        _CardSwapView->close();

        //Show _EndGameView
        if(_EndGameView->isShown()){
            _EndGameView->update();
        }else{
            _cardSelection->remove_all_cards();
            _EndGameView->setIsShown(true);
            _EndGameView->show();
        }

    }else{
        qDebug() << "This gamephase is currently not available!";
    }
}

void GameController::updateSelectedCards(){
    if (_currentGameState->get_gamephase()->get_value() == "game_view"){
        _GameView->updateSelectedCards();
    }else if (_currentGameState->get_gamephase()->get_value() == "switch_view"){
        _CardSwapView->updateSelectedCards();
    }
}

void GameController::playCards(){

    //Check whether dragon has been played
    Card dragon_card = Card(15, 2);
    bool is_dragon = false;
    if ((_cardSelection->number_of_cards() == 1) && (_cardSelection->try_get_card(dragon_card))){
        dragon();
        is_dragon = true;
    }

    if(_cardSelection->number_of_cards() == 0 && !is_dragon){
        GameController::showError("No Card Selected", "Please select a cardcombination before hitting the play button ;)");
    }else if (_cardSelection->is_valid_bomb() && !is_dragon){
        bomb_request request = bomb_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), *_cardSelection);
        _cardSelection->remove_all_cards();
        ClientNetworkManager::sendRequest(request);
        _GameView->buildGameState(_currentGameState, _me);
        _GameView->update();
        _GameView->updateSelectedCards();
    }else if (_cardSelection->is_valid() && !is_dragon){
        play_cards_request request = play_cards_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(),*_cardSelection);
        _cardSelection->remove_all_cards();
        ClientNetworkManager::sendRequest(request);
        _GameView->buildGameState(_currentGameState, _me);
        _GameView->update();
        _GameView->updateSelectedCards();
    } else if(!is_dragon){
        GameController::showError("Invalid Card Selection", "The selection made does not represent a playable combination. Please select a different combination");
    }
}

void GameController::bomb(){
    if(_cardSelection->is_valid_bomb()){
        bomb_request request = bomb_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), *_cardSelection);
        _cardSelection->remove_all_cards();
        ClientNetworkManager::sendRequest(request);
        _GameView->buildGameState(_currentGameState, _me);
        _GameView->update();
        _GameView->updateSelectedCards();
    }else{
        GameController::showError("Invalid Bomb", "This card selection is not a bomb. Please select a valid bomb!");
    }
}
void GameController::exitGame(){
    exit_game_request request = exit_game_request(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}
void GameController::getCards(int nof_cards){
    get_cards_request request = get_cards_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), nof_cards);
    ClientNetworkManager::sendRequest(request);
}

void GameController::joinTeam(std::string team){
    join_team_request request = join_team_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), team);
    ClientNetworkManager::sendRequest(request);
}

void GameController::newGame(){
    new_game_request request = new_game_request(GameController::_currentGameState->get_id(),GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}
void GameController::pass(){
    pass_request request = pass_request(GameController::_currentGameState->get_id(),GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}
void GameController::award_cards(CardCollection giftCard,std::string target_player_id){
    award_cards_request request = award_cards_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(),giftCard,target_player_id);
    ClientNetworkManager::sendRequest(request);
}
void GameController::startGame(){
    start_game_request request = start_game_request(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}
void GameController::startRound(){
    qDebug() << "Start Round Request sent";
    start_round_request request = start_round_request(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}
void GameController::swapCards(CardCollection card_collection){
    swap_cards_request request = swap_cards_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), card_collection);
    ClientNetworkManager::sendRequest(request);
}

void GameController::tichuDeclaration(std::string tichuType){
    tichu_request request = tichu_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(),tichuType);
    ClientNetworkManager::sendRequest(request);
}

void GameController::dragon(){

    std::vector<Player> players = _currentGameState->get_players_ordered(*_me);

    Player* selectedPlayer = new Player;

    if(players[1].get_handcards().size() == 0){
        selectedPlayer = &players[3];
    }else if (players[3].get_handcards().size() == 0){
        selectedPlayer = &players[1];
    }else{
        selectedPlayer = showDragonPopUp(&players[1], &players[3]);
    }
    
    
    play_dragon_request request = play_dragon_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), *_cardSelection, selectedPlayer->get_id());
    _cardSelection->remove_all_cards();
    ClientNetworkManager::sendRequest(request);
}

void GameController::showError(const std::string& title, const std::string& message){
    QMessageBox msgBox;
    msgBox.warning(nullptr, QString::fromStdString(title), QString::fromStdString(message));
    //msgBox.critical(0, QString::fromStdString(title), QString::fromStdString(message));
    msgBox.setFixedSize(500,200);
}

Player* GameController::showDragonPopUp(Player* player1, Player* player2){

    //Create PopUp
    QMessageBox msgBox;
    msgBox.setWindowTitle("Dragon was played!");
    msgBox.setText("Which enemy player would you like to give the points of the stack to?");
    QPushButton *leftPlayerButton = msgBox.addButton(QString::fromStdString(player1->get_name()), QMessageBox::NoRole);
    QPushButton *rightPlayerButton = msgBox.addButton(QString::fromStdString(player2->get_name()), QMessageBox::YesRole);   
    msgBox.setDefaultButton(rightPlayerButton);
    msgBox.exec();

    //Returns Clicked Player
    if(msgBox.clickedButton() == rightPlayerButton){
        return player2;      
    }else{
        return player1;
    }
}

GameState* GameController::getCurrentGameState() {
    return _currentGameState;
}

Player* GameController::getMe() {
    return _me;
}

void GameController::showStatus(const std::string& message){

}

void GameController::showNewRoundMessage(GameState* oldGameState, GameState* newGameState){

}

void GameController::showGameOverMessage(){

}
