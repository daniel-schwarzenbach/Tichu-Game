//This panel is shown after every round and displayes round- and gamepoints and the winner of the round

#include "EndRoundView.h"
#include "./ui_EndRoundView.h"
#include "../GameController.h"


EndRoundView::EndRoundView(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::EndRoundView)
{
    ui->setupUi(this);
}



//Update the EndRoundView with the current GameState
void EndRoundView::updateGameState(GameState* gameState, Player* _me){
    std::vector<Player> players = gameState->get_players_ordered(*_me);

    //color and font size parameters
    QString red= "#cc0000";
    QString blue= "#3465a4";
    QString fSize="14pt";

    //extract the points of the last round played
    std::vector<serializable_value<int>> pointTableDragon = gameState->get_teams().at(0).get_points_vector();
    std::vector<serializable_value<int>> pointTablePhoenix = gameState->get_teams().at(1).get_points_vector();
    int dragonRoundPoints= pointTableDragon.at(pointTableDragon.size()-1).get_value();
    int phoenixRoundPoints=pointTablePhoenix.at(pointTablePhoenix.size()-1).get_value();

    //set retrieved round scores to the UI and style them
    ui->dragon_points_tot->setText(QString::number(dragonRoundPoints));
    ui->dragon_points_tot->setStyleSheet("QLabel {color: "+red+"; qproperty-alignment: 'AlignCenter'; font: "+fSize+"}");
    ui->phoenix_points_tot->setText(QString::number(phoenixRoundPoints));
    ui->phoenix_points_tot->setStyleSheet("QLabel {color: "+blue+"; qproperty-alignment: 'AlignCenter'; font: "+fSize+"}");

    //set retrieved game scores to the UI and style them
    ui->dragon_game_scores->setText(QString::number(gameState->get_teams().at(0).get_points()));
    ui->dragon_game_scores->setStyleSheet("QLabel {color: "+red+"; qproperty-alignment: 'AlignCenter'; font: "+fSize+"}");
    ui->phoenix_game_scores->setText(QString::number(gameState->get_teams().at(1).get_points()));
    ui->phoenix_game_scores->setStyleSheet("QLabel {color: "+blue+"; qproperty-alignment: 'AlignCenter'; font: "+fSize+"}");

    //display the winner of the round
    if(dragonRoundPoints>phoenixRoundPoints){
        ui->win_message->setText("<span style='color: "+red+";'>Team Dragon Wins Round!</span>");
    }
    else if(dragonRoundPoints<phoenixRoundPoints){
        ui->win_message->setText("<span style='color: "+blue+";'>Team Phoenix Wins Round!</span>");
    }
    else{
        ui->win_message->setText("<span style='color: #000000;'>Tie!</span>");
    }
    ui->win_message->setStyleSheet("QLabel {qproperty-alignment: 'AlignCenter'; font: 18pt;}");
}


void EndRoundView::on_newRoundButton_clicked()
{
    qDebug() << "Start Game Request sent";
    GameController::startGame();
}

EndRoundView::~EndRoundView()
{
    delete ui;
}

bool EndRoundView::isShown(){
    return _isShown;
}

void EndRoundView::setIsShown(bool isShown){
    _isShown = isShown;
}

