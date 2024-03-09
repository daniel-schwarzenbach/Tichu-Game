#include "CardSwapView.h"
#include "./ui_CardSwapView.h"

#include "../GameController.h"
#include <QGraphicsDropShadowEffect>

CardSwapView::CardSwapView(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::CardSwapView),
    _handScene(nullptr),
    _cardsPlayer1(nullptr),
    _cardsPlayer2(nullptr),
    _cardsPlayer3(nullptr),
    _gamephase("")
{
    ui->setupUi(this);

    //initialize Graphicsscenes
    _cardsPlayer1 = new QGraphicsScene(this);
    _cardsPlayer2 = new QGraphicsScene(this);
    _cardsPlayer3 = new QGraphicsScene(this);

    //Create array of all the views so code is a lot shorter
    QGraphicsView *all_Views[4] = {ui->HandView, ui->Player1CardView, ui->Player2CardView, ui->Player3CardView};

    //Some settings from all the GraphicsViews, so that they have no background nor border
    for (int i = 0; i < 4; i++){
        all_Views[i]->setFrameShape(QFrame::NoFrame);
        all_Views[i]->setStyleSheet("background: transparent");
    }

    //Adds all the GraphicScenes to the ui
    ui->Player1CardView->setScene(_cardsPlayer1);
    ui->Player2CardView->setScene(_cardsPlayer2);
    ui->Player3CardView->setScene(_cardsPlayer3);

    //Remove Swap Indicator Text 
    ui->Player1SwapLabel->setText("");
    ui->Player2SwapLabel->setText("");
    ui->Player3SwapLabel->setText("");

    //Initialize selected Cards vector
    _selectedCards = std::vector<Card>(3);
}

CardSwapView::~CardSwapView()
{
    delete ui;
    delete _handScene;
    delete _cardsPlayer1;
    delete _cardsPlayer2;
    delete _cardsPlayer3;
}

void CardSwapView::buildGameState(GameState* gameState, Player* me){
    if(me->get_handcards().size() == 8){
        _gamephase = "big_tichu";
        ui->TitleLabel->setText("Declaration of Big Tichu");
        ui->ContinueButton->setText("No Tichu");
        ui->TichuButton->setText("Big Tichu");
        
        //The following few lines are used when a new round is started after the first one
        //Remove Swap Indicator
        ui->Player1SwapLabel->clear();
        ui->Player2SwapLabel->clear();
        ui->Player3SwapLabel->clear();

        //Overwrite selected Card vector with empty Cards
        _selectedCards = std::vector<Card>(3);

        //ContinueButton should be visible
        ui->ContinueButton->show();
    }else{
        _gamephase = "swap";
        ui->TitleLabel->setText("Exchanging Cards");
        ui->ContinueButton->setText("Continue");
        ui->TichuButton->setText("Tichu");
    }

    //build all Subsections
    buildPlayerHand(gameState, me);
    buildOtherPlayers(gameState, me);
}

void CardSwapView::buildPlayerHand(GameState* gameState, Player* me){

    //destroys old GraphicsScene and makes a new one
    generateNewGraphicsScene(&_handScene, ui->HandView);

    //Sets text of Playername
    ui->myPlayerNameLabel->setText(QString::fromStdString(me->get_name()));

    //Changes Playername Color to match Team Color
    if(me->get_teamID() == "dragon"){
        ui->myPlayerNameLabel->setStyleSheet("QLabel {color : rgb(236,44,51); }"); //Red
    }else{
        ui->myPlayerNameLabel->setStyleSheet("QLabel {color : rgb(10,131,224); }"); //Blue
    }

    //Adds Tichulabel and Seperation Line when needed
    if(me->get_big_tichu()){
        ui->TichuLabel->show();
        ui->TichuLabel->setText("TICHU");
        ui->SeperationLine->show();
        ui->myPlayerNameLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    }else if(me->get_small_tichu()){
        ui->TichuLabel->show();
        ui->TichuLabel->setText("Tichu");
        ui->SeperationLine->show();
        ui->myPlayerNameLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    }else{
        ui->SeperationLine->hide();
        ui->TichuLabel->hide();
        ui->myPlayerNameLabel->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    }

    std::vector<Card> handCards = me->get_handcards();

    //Erases all card which are already swapped from player hand
    for (const auto& card : _selectedCards) {
        auto it = std::find_if(handCards.begin(), handCards.end(), [&card](const Card& c) {
            return c.get_type() == card.get_type() and c.get_family() == card.get_family();
        });
        if (it != handCards.end()) {
            handCards.erase(it);
        }
    }

    //Disable Continue Button if not all cards have been swapped
    if(_selectedCards[0].isEmptyCard() || _selectedCards[1].isEmptyCard() || _selectedCards[2].isEmptyCard()){
        ui->ContinueButton->setEnabled(false);
    }else{
        ui->ContinueButton->setEnabled(true);
    }

    //Continue Button should always be enabled when in big tichu gamephase
    if(_gamephase == "big_tichu"){
        ui->ContinueButton->setEnabled(true);
    }

    //Continue Button should be hidden when player has swapped
    if(me->get_swap_state()){
        ui->ContinueButton->hide();
    }

    //Hide Tichu Button if Tichu is already declared
    if(me->get_small_tichu() || me->get_big_tichu()){
        ui->TichuButton->hide();
    }else{
        ui->TichuButton->show();
    }

    //iterate through Card vector to display it accordingly
    for(int i = 0; i < handCards.size(); ++i){

        //Add QGraphicsDropShadowEffect to improve visibility.
        QGraphicsDropShadowEffect *e = new QGraphicsDropShadowEffect;
        e->setColor(QColor(40,40,40,180));
        e->setOffset(-2,2);
        e->setBlurRadius(30);

        Card current_card = handCards[i];

        int family = current_card.get_family();
        int value = current_card.get_type();

        std::string img_path = "assets/tichu_card_" + std::to_string(family) + "_" + std::to_string(value) + ".png";

        //Adds the card_img to the HandView to be displayed
        QPixmap card_img = QPixmap(QString::fromStdString(img_path));
        CardItem *pm  = new CardItem(value, family, true, true);
        
        //Cards are not selectable when in big tichu gamephase otherwise they are
        if(_gamephase == "big_tichu"){
            pm  = new CardItem(value, family, false);
        }

        pm->setPixmap(card_img);
        pm->setPos(i*300,0);
        pm->setGraphicsEffect(e);
        if(GameController::_cardSelection->try_get_card(current_card)){
            pm->moveBy(0,-100);
            pm->select();
        }
        _handScene->addItem(pm);

        //This is need so that only the selected card moves up and not as before all cards down and the selcted card up, when selecting the first time
        ui->HandView->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    }
}

void CardSwapView::buildOtherPlayers(GameState* gameState, Player* me){

    //clear all the views of existing items
    generateNewGraphicsScene(&_cardsPlayer1, ui->Player1CardView);
    generateNewGraphicsScene(&_cardsPlayer2, ui->Player2CardView);
    generateNewGraphicsScene(&_cardsPlayer3, ui->Player3CardView);

    //create arrays so code gets a lot shorter
    QGraphicsView *allPlayerViews[3] = {ui->Player1CardView, ui->Player2CardView, ui->Player3CardView};
    QGraphicsScene *playerCardScenes[3] = {_cardsPlayer1, _cardsPlayer2, _cardsPlayer3};
    QLabel *allPlayerNameLabels[3] = {ui->Player1Label, ui->Player2Label, ui->Player3Label};
    QLabel *allPlayerTichuLabels[3] = {ui->Player1TichuLabel, ui->Player2TichuLabel, ui->Player3TichuLabel};
    QLabel *allPlayerSwapLabels[3] = {ui->Player1SwapLabel, ui->Player2SwapLabel, ui->Player3SwapLabel};
    QPushButton *allSwapButtons[3] = {ui->swapButton1, ui->swapButton2, ui->swapButton3};

    std::vector<Player> players = gameState->get_players_ordered(*me);

    //import image of card_back and Checkmark icon with white box as pixmap
    QPixmap cardback_img = QPixmap("assets/no_card.png");
    QPixmap checkmark_img = QPixmap("assets/Checkmark.png");

    //in the players vector i = 0 is the user of the client this is why i+1 is used at this point
    for (int i = 0; i < 3; i++){

        //Change text to Playername
        allPlayerNameLabels[i]->setText(QString::fromStdString(players[i+1].get_name()));

        //Change text of buttons to match the Playername
        QString button_text = "Swap to " + QString::fromStdString(players[i+1].get_name());
        allSwapButtons[i]->setText(button_text);

        //Disable Swap Buttons when in big tichu gamephase
        if(_gamephase == "big_tichu"){
            allSwapButtons[i]->setEnabled(false);
        }else{
            allSwapButtons[i]->setEnabled(true);
        }

        //Shows whether a Player has declared a Tichu or not
        if(players.at(i+1).get_big_tichu()){
            allPlayerTichuLabels[i]->setText("TICHU");
        }else if(players.at(i+1).get_small_tichu()){
            allPlayerTichuLabels[i]->setText("Tichu");
        }else{
            allPlayerTichuLabels[i]->setText("");
        }

        //Change Playername Color to match Team Color
        if(players.at(i+1).get_teamID() == "dragon"){
            allPlayerNameLabels[i]->setStyleSheet("QLabel {color : rgb(236,44,51); }"); //Red
        }else{
            allPlayerNameLabels[i]->setStyleSheet("QLabel {color : rgb(10,131,224); }"); //Blue
        }

        //When Player has swappedhis cards, a checkmark is displayed next to his name
        if(players.at(i+1).get_swap_state()){
            allPlayerSwapLabels[i]->setPixmap(checkmark_img.scaled(allPlayerSwapLabels[i]->width(), allPlayerSwapLabels[i]->height(), Qt::KeepAspectRatio));
        }

        //add image of card or placeholder to the scene
        QGraphicsPixmapItem *pm  = new QGraphicsPixmapItem;

        //If card selection is empty display placeholder
        if(_selectedCards[i].isEmptyCard()){
            pm->setPixmap(cardback_img);
        }else{
            //Add QGraphicsDropShadowEffect to improve visibility.
            QGraphicsDropShadowEffect *e = new QGraphicsDropShadowEffect;
            e->setColor(QColor(40,40,40,180));
            e->setOffset(-2,2);
            e->setBlurRadius(30);

            int family = _selectedCards[i].get_family();
            int type = _selectedCards[i].get_type();
            std::string img_path = "assets/tichu_card_" + std::to_string(family) + "_" + std::to_string(type) + ".png";
            pm->setPixmap(QPixmap(QString::fromStdString(img_path)));
            pm->setGraphicsEffect(e);
        }     

        playerCardScenes[i]->addItem(pm);

        //Alignments
        if(i == 0){
            allPlayerViews[i]->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        }else if (i == 1){
            allPlayerViews[i]->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        }else{
            allPlayerViews[i]->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        }
    }   
}

void CardSwapView::resizeEvent(QResizeEvent *event){

    //create array of all player card views and scenes so code gets a lot shorter
    QGraphicsScene *playerCardScenes[3] = {_cardsPlayer1, _cardsPlayer2, _cardsPlayer3};
    QGraphicsView *allPlayerViews[3] = {ui->Player1CardView, ui->Player2CardView, ui->Player3CardView};

    for (int i = 0; i < 3; ++i) {
        allPlayerViews[i]->fitInView(QRectF(0.0, 0.0, playerCardScenes[i]->itemsBoundingRect().size().width()+60, playerCardScenes[i]->itemsBoundingRect().size().height()+100) ,Qt::KeepAspectRatio);
    }

    //How to handle resizing of Hand and CurrentStack
    ui->HandView->fitInView(QRectF(0.0, 0.0, _handScene->itemsBoundingRect().size().width()+60, _handScene->itemsBoundingRect().size().height()+100) ,Qt::KeepAspectRatio);
    
    QWidget::resizeEvent(event);
}

//Used to generate a new GraphicsView. Doing it this way ensures that cards will always stay centered
void CardSwapView::generateNewGraphicsScene(QGraphicsScene** graphicsScene, QGraphicsView* graphicsView){
    if (graphicsView != nullptr){
        delete *graphicsScene;   
    }
    *graphicsScene = new QGraphicsScene(this);
    graphicsView->setScene(*graphicsScene);
}

bool CardSwapView::isShown(){
    return _isShown;
}

void CardSwapView::setIsShown(bool isShown){
    _isShown = isShown;
}

void CardSwapView::on_swapButton1_clicked()
{   
    if(GameController::_cardSelection->number_of_cards() == 0){
        GameController::showError("No Cards", "Please select a card before swapping!");
    }else if(GameController::_cardSelection->number_of_cards() == 1){
        //add selected Card to _selectedCards vector at the correct position
        Card selectedCard = GameController::_cardSelection->get_cards()[0];
        _selectedCards[0] = selectedCard;

        //Clear Graphic Scene
        _cardsPlayer1->clear();

        //Update GUI
        buildOtherPlayers(GameController::getCurrentGameState(),GameController::getMe());
        buildPlayerHand(GameController::getCurrentGameState(),GameController::getMe());

        GameController::_cardSelection->remove_all_cards();
    }else{
        GameController::showError("Too many Cards", "You have too many cards selected. You should only have one!");
    }
}


void CardSwapView::on_swapButton2_clicked()
{
    if(GameController::_cardSelection->number_of_cards() == 0){
        GameController::showError("No Cards", "Please select a card before swapping!");
    }else if(GameController::_cardSelection->number_of_cards() == 1){
        //add selected Card to _selectedCards vector at the correct position
        Card selectedCard = GameController::_cardSelection->get_cards()[0];
        _selectedCards[1] = selectedCard;

        //Clear Graphic Scene
        _cardsPlayer2->clear();
        
        //Update GUI
        buildOtherPlayers(GameController::getCurrentGameState(),GameController::getMe());
        buildPlayerHand(GameController::getCurrentGameState(),GameController::getMe());

        GameController::_cardSelection->remove_all_cards();
    }else{
        GameController::showError("Too many Cards", "You have too many cards selected. You should only have one!");
    }
}


void CardSwapView::on_swapButton3_clicked()
{
    if(GameController::_cardSelection->number_of_cards() == 0){//add selected Card to _selectedCards vector at the correct position
        GameController::showError("No Cards", "Please select a card before swapping!");
    }else if(GameController::_cardSelection->number_of_cards() == 1){
        //add selected Card to _selectedCards vector at the correct position
        Card selectedCard = GameController::_cardSelection->get_cards()[0];
        _selectedCards[2] = selectedCard;

        //Clear Graphic Scene
        _cardsPlayer3->clear();

        //Update GUI
        buildOtherPlayers(GameController::getCurrentGameState(),GameController::getMe());
        buildPlayerHand(GameController::getCurrentGameState(),GameController::getMe());

        GameController::_cardSelection->remove_all_cards();
    }else{
        GameController::showError("Too many Cards", "You have too many cards selected. You should only have one!");
    }
}

void CardSwapView::updateSelectedCards(){
    ui->HandView->updateSelectedCards();
}


void CardSwapView::on_ContinueButton_clicked()
{   
    if(_gamephase == "big_tichu"){
        GameController::tichuDeclaration("");
    }else{
        CardCollection swapCards = CardCollection(_selectedCards);
        GameController::swapCards(swapCards);
    }
}


void CardSwapView::on_TichuButton_clicked()
{
    if(_gamephase == "big_tichu"){
        GameController::tichuDeclaration("big tichu");
    }else{
        GameController::tichuDeclaration("small tichu");
    }
}