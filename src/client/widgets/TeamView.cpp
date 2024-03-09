#include "TeamView.h"
#include "ui_TeamView.h"
#include "../GameController.h"



TeamView::TeamView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::layout),
    _bluecount(0),
    _redcount(0),
    _me(Player()),
    _isShown(false)

{
    ui->setupUi(this);

    _Middle = std::vector<QLabel*>{ui->labelMiddle1, ui->labelMiddle2, ui->labelMiddle3, ui->labelMiddle4};
    _Left = std::vector<QLabel*>{ui->labelLeft1, ui->labelLeft2, ui->labelLeft3, ui->labelLeft4};
    _Right = std::vector<QLabel*>{ui->labelRight1, ui->labelRight2, ui->labelRight3, ui->labelRight4};

    //Remove all text
    for(int i=0; i<4;i++){
        _Middle[i]->setText("");
        _Left[i]->setText("");
        _Left[i]->setStyleSheet("QLabel {color : rgb(236,44,51); }"); //Red
        _Right[i]->setText("");
        _Right[i]->setStyleSheet("QLabel {color : rgb(10,131,224); }"); //Blue
    }

    ui->teamBlueLabel->setStyleSheet("QLabel {color : rgb(10,131,224); }");
    ui->teamRedLabel->setStyleSheet("QLabel {color :rgb(236,44,51); }");

    //Set Icon for Left Button
    QPixmap pixmap1("assets/LeftArrowIcon.png");
    QIcon ButtonIcon1(pixmap1);
    ui->LeftButton->setIcon(ButtonIcon1);
    ui->LeftButton->setText("");
    ui->LeftButton->setIconSize(QSize(60, 20));
    ui->LeftButton->setFlat(true);

    //Set Icon for Right Button
    QPixmap pixmap2("assets/RightArrowIcon.png");
    QIcon ButtonIcon2(pixmap2);
    ui->RightButton->setIcon(ButtonIcon2);
    ui->RightButton->setText("");
    ui->RightButton->setIconSize(QSize(60, 20));
    ui->RightButton->setFlat(true);
}

TeamView::~TeamView()
{
    delete ui;
}

void TeamView::buildGameState(GameState* gameState, Player* me)
{   
    //get players from new gamestate
    _me = *me;
    std::vector<Player> players = gameState->get_players_ordered(*me);

    qDebug() << this->_bluecount;

    //Reset Count
    _bluecount = 0;
    _redcount = 0;

    //Check if four player in game and client player chose a team
    if(players.size() == 4 && position != "middle"){

        //Check if all other players are in a Team
        if(players[1].get_teamID() != "" && players[2].get_teamID() != "" && players[3].get_teamID() != ""){
            ui->startButton->setEnabled(true);
        }else{
            ui->startButton->setEnabled(false);
        }

    }else{
        ui->startButton->setEnabled(false);
    }

    for(int i = 0; i < 4; ++i){
        //Remove Text
        _Middle[i]->setText("");
        _Left[i]->setText("");
        _Right[i]->setText("");
    }

    for(int i = 0; i< players.size(); ++i){
        //Write Playername at correct position
        if(players[i].get_teamID() == ""){
            _Middle[i]->setText(QString::fromStdString(players[i].get_name()));
        }else if(players[i].get_teamID() == "dragon"){
            _redcount++;
            _Left[i]->setText(QString::fromStdString(players[i].get_name()));
        }else{
            _bluecount++;
            _Right[i]->setText(QString::fromStdString(players[i].get_name()));
        }
    }

    //Disable Left and Right Button if needed
    if((_bluecount > 1 && position == "middle") || position == "right"){
        ui->RightButton->setEnabled(false);
    }else{
       ui->RightButton->setEnabled(true);
    }

    if((_redcount > 1 && position == "middle") || position == "left"){
        ui->LeftButton->setEnabled(false);
    }else{
        ui->LeftButton->setEnabled(true);
    }
}

void TeamView::on_LeftButton_clicked()
{
    if(position == "right"){
        position = "middle";

        //Leave Team
        GameController::joinTeam("");
    }else if (position == "left"){
    }else{
        position = "left";

        //Join Team
        GameController::joinTeam("dragon");
    }
}

void TeamView::on_RightButton_clicked()
{
    if(position == "right"){
    }else if (position == "left"){
        position = "middle";

        //Leave Team
        GameController::joinTeam("");
    }else{
        position = "right";

        //Join Team
        GameController::joinTeam("phoenix");
    }
}

void TeamView::on_startButton_clicked(){
    GameController::startGame();
}

bool TeamView::isShown(){
    return _isShown;
}

void TeamView::setIsShown(bool isShown){
    _isShown = isShown;
}