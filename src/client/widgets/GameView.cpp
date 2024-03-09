#include "GameView.h"
#include "./ui_GameView.h"

#include "../GameController.h"
#include <QGraphicsDropShadowEffect>

GameView::GameView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameView),
    _currentStackScene(nullptr),
    _handScene(nullptr),
    _cardsPlayer1(nullptr),
    _cardsPlayer2(nullptr),
    _cardsPlayer3(nullptr)
{
    ui->setupUi(this);

    //initialize Graphicsscenes
    _cardsPlayer1 = new QGraphicsScene(this);
    _cardsPlayer2 = new QGraphicsScene(this);
    _cardsPlayer3 = new QGraphicsScene(this);

    //Create array of all the views so code is a lot shorter
    QGraphicsView *all_Views[5] = {ui->HandView, ui->CurrentStackView, ui->CardsPlayer1, ui->CardsPlayer2, ui->CardsPlayer3};

    //Some settings from the all the GraphicsViews, so that the QGraphicsView has no background nor border
    for (int i = 0; i < 5; i++){
        all_Views[i]->setFrameShape(QFrame::NoFrame);
        all_Views[i]->setStyleSheet("background: transparent");
    }

    //Adds all the GraphicScenes to the ui
    ui->CardsPlayer1->setScene(_cardsPlayer1);
    ui->CardsPlayer2->setScene(_cardsPlayer2);
    ui->CardsPlayer3->setScene(_cardsPlayer3);

    //Set Icon for Chat Button
    QPixmap pixmap1("assets/ChatIcon.png");
    QIcon ButtonIcon1(pixmap1);
    ui->chatButton->setIcon(ButtonIcon1);
    ui->chatButton->setText("");
    ui->chatButton->setIconSize(QSize(60, 50));
    ui->chatButton->setFlat(true);

    //Set Icon for Deselect Button
    QPixmap pixmap2("assets/DeselectIcon.png");
    QIcon ButtonIcon2(pixmap2);
    ui->deselectButton->setIcon(ButtonIcon2);
    ui->deselectButton->setText("");
    ui->deselectButton->setIconSize(QSize(60, 50));
    ui->deselectButton->setFlat(true);
}

GameView::~GameView()
{
    delete ui;
    delete _currentStackScene;
    delete _handScene;
    delete _cardsPlayer1;
    delete _cardsPlayer2;
    delete _cardsPlayer3;
}

void GameView::buildGameState(GameState* gameState, Player* me){
    //build all subelements of the gameview
    buildPlayerHand(gameState, me);
    buildTurnIndicator(gameState, me);
    buildCurrentStack(gameState, me);
    buildOtherPlayers(gameState, me);

    //Checks whether it is my turn or not
    if(gameState->get_player_index(*me) == gameState->get_current_player_index()->get_value()){
        //Auto Pass when less cards in Hand then on table and smaller then 4. This is because a bomb could still be played on a street
        if((me->get_handcards().size() < gameState->get_cards().size()) && (me->get_handcards().size() < 4)){
            GameController::pass();
        }
    }
}

void GameView::buildPlayerHand(GameState* gameState, Player* me){

    //destroys old GraphicsScene and makes new one
    generateNewGraphicsScene(&_handScene, ui->HandView);

    //Set text of Playername and Points
    ui->myPlayerNameLabel->setText(QString::fromStdString(me->get_name()));
    ui->myPlayerPoints->setText(QString::fromStdString(std::to_string(me->get_points()) + " Points"));

    //Change Playername color to match teams color
    if(me->get_teamID() == "dragon"){
        ui->myPlayerNameLabel->setStyleSheet("QLabel {color : rgb(236,44,51); }"); //Red
    }else{
        ui->myPlayerNameLabel->setStyleSheet("QLabel {color : rgb(10,131,224); }"); //Blue
    }

    //Adds Tichulabel and Seperation Line when needed
    if(me->get_big_tichu()){
        ui->TichuLabel->show();
        ui->TichuLabel->setText("TICHU");
        ui->SeprationLine->show();
        ui->myPlayerPoints->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    }else if(me->get_small_tichu()){
        ui->TichuLabel->show();
        ui->TichuLabel->setText("Tichu");
        ui->SeprationLine->show();
        ui->myPlayerPoints->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    }else{
        ui->SeprationLine->hide();
        ui->TichuLabel->hide();
        ui->myPlayerPoints->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    }

    //Set default settings for Buttons
    ui->bombButton->setEnabled(true);
    ui->chatButton->setEnabled(true);
    ui->bombButton->setEnabled(true);
    ui->bombButton->show();

    std::vector<Card> handCards = me->get_handcards();

    //The Tichu Button gets removed when the first card is played and will no longer be visible
    if(handCards.size() == 14 && !me->get_small_tichu() && !me->get_big_tichu()){
        ui->tichuButton->show();
    }else if(handCards.size() == 0){
        ui->bombButton->hide();
    }else{
        ui->tichuButton->hide();
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
        CardItem *pm  = new CardItem(value, family, true);
        pm->setPixmap(card_img);
        pm->setPos(i*300,0);
        pm->setGraphicsEffect(e);
        _handScene->addItem(pm);

        //This is need so that only the selected card moves up and not as before all cards down and the selcted card up, when selecting the first time
        ui->HandView->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    }
}

void GameView::buildTurnIndicator(GameState* gameState, Player* me){
    int turnidx = gameState->get_current_player_index()->get_value();
    std::vector<Player> players = gameState->get_players_ordered(*me);

    //Change text of TurnLabel so the user knows whos turn it is
    for(int i = 0; i < players.size(); i++){
        if(gameState->is_current_player(players[i]) && i == 0){
            ui->TurnLabel->setText("It's your turn!");
            ui->playButton->show();
            ui->passButton->show();
        }else if(gameState->is_current_player(players[i])){
            std::string text = "It's " + players[i].get_name() + "'s turn.";
            ui->TurnLabel->setText(QString::fromStdString(text));
            ui->playButton->hide();
            ui->passButton->hide();
        }
    }
}

void GameView::buildCurrentStack(GameState* gameState, Player* me){
    //destroys old GraphicsScene and makes new one
    generateNewGraphicsScene(&_currentStackScene, ui->CurrentStackView);

    //get vector of cards which are currently on display
    std::vector<Card> cards_on_display = gameState->get_cards();

    //Display how much points are currently on the table and can be won
    std::string text = "Something went wrong";
    if(gameState->get_stack_points() == 0){
        text = "No points available to win";
    }else{
        text = "Points avaible to win: " + std::to_string(gameState->get_stack_points());
    }
    ui->StackPointLabel->setText(QString::fromStdString(text));

    //One should not be able to pass or bomb when no cards or the dog is displayed
    if(cards_on_display.size() == 0 || (cards_on_display[0].get_family() == 4 && cards_on_display[0].get_type() == 15)){
        
        //Adds the no cards image to the currentStackView when no cards are displayed
        if(cards_on_display.size() == 0){
            QPixmap card_img = QPixmap("assets/no_cards.png");
            QGraphicsPixmapItem *pm = new QGraphicsPixmapItem();
            pm->setPixmap(card_img);
            _currentStackScene->addItem(pm);
        }

        //Hide Bomb and Pass Button
        ui->bombButton->hide();
        ui->passButton->hide();
    }

    //iterate through Card vector to display it accordingly
    for(int i = 0; i < cards_on_display.size(); ++i){

        //Add QGraphicsDropShadowEffect to improve visibility.
        QGraphicsDropShadowEffect *e = new QGraphicsDropShadowEffect;
        e->setColor(QColor(40,40,40,180));
        e->setOffset(-2,2);
        e->setBlurRadius(20);

        Card current_card = cards_on_display[i];

        int family = current_card.get_family();
        int value = current_card.get_type();

        std::string img_path = "assets/tichu_card_" + std::to_string(family) + "_" + std::to_string(value) + ".png";

        //Adds the card_img to the currentStackView to be displayed
        QPixmap card_img = QPixmap(QString::fromStdString(img_path));
        CardItem *pm  = new CardItem(value, family, false);
        pm->setPixmap(card_img);
        pm->setPos(i*500,0);
        pm->setGraphicsEffect(e);
        _currentStackScene->addItem(pm);
    }
}

void GameView::buildOtherPlayers(GameState* gameState, Player* me){

    //clear all the views of existing items
    generateNewGraphicsScene(&_cardsPlayer1, ui->CardsPlayer1);
    generateNewGraphicsScene(&_cardsPlayer2, ui->CardsPlayer2);
    generateNewGraphicsScene(&_cardsPlayer3, ui->CardsPlayer3);

    //create arrays so code gets a lot shorter
    QGraphicsView *allPlayerViews[3] = {ui->CardsPlayer1, ui->CardsPlayer2, ui->CardsPlayer3};
    QGraphicsScene *playerCardScenes[3] = {_cardsPlayer1, _cardsPlayer2, _cardsPlayer3};
    QLabel *allPlayerNameLabels[3] = {ui->Player1NameLabel, ui->Player2NameLabel, ui->Player3NameLabel};
    QLabel *allPlayerPointsLabels[3] = {ui->Player1PointsLabel, ui->Player2PointsLabel, ui->Player3PointsLabel};
    QLabel *allPlayerTichuLabels[3] = {ui->Player1TichuLabel, ui->Player2TichuLabel, ui->Player3TichuLabel};

    //get all the player objects in an ordered fashion starting with me
    std::vector<Player> players = gameState->get_players_ordered(*me);

    //import image of card_back with white box as pixmap
    QPixmap cardback_img = QPixmap("assets/card_back_with_rect.png");

    //i = 0 is the player of the client that is why all players will be extracted with i + 1
    for (int i = 0; i < 3; i++){

        //Set the text of the according textlabel to the Name and Points of Player i 
        allPlayerNameLabels[i]->setText(QString::fromStdString(players[i+1].get_name()));
        std::string point_str = std::to_string(players[i+1].get_points()) + " Points";
        allPlayerPointsLabels[i]->setText(QString::fromStdString(point_str));

        //Change Playername color to match teams color
        if(players[i+1].get_teamID() == "dragon"){
            allPlayerNameLabels[i]->setStyleSheet("QLabel {color : rgb(236,44,51); }"); //Red
        }else{
            allPlayerNameLabels[i]->setStyleSheet("QLabel {color : rgb(10,131,224); }"); //Blue
        }

        //Set text of tichulabel display tichustate of player
        if(players[i+1].get_big_tichu()){
            allPlayerTichuLabels[i]->setText("TICHU");
        }else if(players[i+1].get_small_tichu()){
            allPlayerTichuLabels[i]->setText("Tichu");
        }else{
            allPlayerTichuLabels[i]->setText("");
        }

        //add CardBack image to the GraphicScene
        QGraphicsPixmapItem *pm  = new QGraphicsPixmapItem;
        pm->setPixmap(cardback_img);
        pm->setPos(0,0);
        playerCardScenes[i]->addItem(pm);

        //adds Number of Cards to the middle of Image
        int NumCards = players[i+1].get_handcards().size();
        QGraphicsTextItem *n_cards = new QGraphicsTextItem(QString::number(NumCards));
        QFont f;
        f.setPointSize(200);
        f.setBold(true);
        n_cards->setFont(f);

        //sets position to the number ot the middle of the Image
        int x_pos = pm->boundingRect().size().width()/2 - n_cards->boundingRect().size().width()/2;
        int y_pos = pm->boundingRect().size().height()/2 - n_cards->boundingRect().size().width()/2;
        n_cards->setPos(x_pos, y_pos);
        playerCardScenes[i]->addItem(n_cards);

        //Set alignments of the graphicsScene in GraphicsView
        if(i == 0){
            allPlayerViews[i]->setAlignment(Qt::AlignBottom|Qt::AlignRight);
        }else if (i == 1){
            allPlayerViews[i]->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        }else{
            allPlayerViews[i]->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
        }
    }   
}

void GameView::resizeEvent(QResizeEvent *event){

    //create array of all player card views and scenes so code gets a lot shorter
    QGraphicsScene *playerCardScenes[3] = {_cardsPlayer1, _cardsPlayer2, _cardsPlayer3};
    QGraphicsView *allPlayerViews[3] = {ui->CardsPlayer1, ui->CardsPlayer2, ui->CardsPlayer3};

    for (int i = 0; i < 3; ++i) {
        allPlayerViews[i]->fitInView(QRectF(0.0, 0.0, playerCardScenes[i]->itemsBoundingRect().size().width(), playerCardScenes[i]->itemsBoundingRect().size().height()) ,Qt::KeepAspectRatio);
    }

    //How to handle resizing of Hand and CurrentStack
    ui->HandView->fitInView(QRectF(0.0, 0.0, _handScene->itemsBoundingRect().size().width()+60, _handScene->itemsBoundingRect().size().height()+100) ,Qt::KeepAspectRatio);
    ui->CurrentStackView->fitInView(QRectF(0.0, 0.0, _currentStackScene->itemsBoundingRect().size().width()+60, _currentStackScene->itemsBoundingRect().size().height()+100) ,Qt::KeepAspectRatio);

    QWidget::resizeEvent(event);
}

//Used to generate a new GraphicsView. Doing it this way ensures that cards will always stay centered (this was a bug before)
void GameView::generateNewGraphicsScene(QGraphicsScene** graphicsScene, QGraphicsView* graphicsView){
    if (graphicsView != nullptr){
        delete *graphicsScene;   
    }
    *graphicsScene = new QGraphicsScene(this);
    graphicsView->setScene(*graphicsScene);
}

void GameView::updateSelectedCards(){
    ui->HandView->updateSelectedCards();

    //When no cards are selected, then the deselect button gets disabled
    if(ui->HandView->getNSelectedCards() != 0){
        ui->deselectButton->setEnabled(true);
    }else{
        ui->deselectButton->setEnabled(false);
    }
}

void GameView::on_tichuButton_clicked()
{
    GameController::tichuDeclaration("small tichu");
}


void GameView::on_passButton_clicked()
{
    GameController::pass();
}


void GameView::on_playButton_clicked()
{
    GameController::playCards();
}


void GameView::on_bombButton_clicked()
{
    GameController::bomb();
}

bool GameView::isShown(){
    return _isShown;
}

void GameView::setIsShown(bool isShown){
    _isShown = isShown;
}

void GameView::on_chatButton_clicked()
{
    //Open Chat Window
    //TODO KENNY
}

void GameView::on_deselectButton_clicked()
{
    GameController::_cardSelection->remove_all_cards();
    GameController::updateViews();
}

