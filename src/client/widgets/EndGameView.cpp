//
// Created by dominic on 16.05.23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EndGameView.h" resolved

#include "EndGameView.h"
#include "ui_EndGameView.h"
#include <QApplication>
#include <QTableWidget>
#include <QScrollBar>
#include <iostream>
#include <vector>
#include <array>
#include <QStandardItemModel>
#include "../GameController.h"

EndGameView::EndGameView(QWidget *parent) :
    QWidget(parent), ui(new Ui::EndGameView) {
    ui->setupUi(this);
}

void EndGameView::updateGameState(GameState* gameState, Player* _me) {
    //vector with the three Colors used in table
    std::vector<QColor> colors = {"black", "#cc0000", "#3465a4"};

    ui->scoreTable->verticalHeader()->setVisible(false);

    std::vector<serializable_value<int>> pointTableA = gameState->get_teams().at(0).get_points_vector();
    std::vector<serializable_value<int>> pointTableB = gameState->get_teams().at(1).get_points_vector();
    int final_score_Dragon = gameState->get_teams().at(0).get_points();
    int final_score_Phoenix = gameState->get_teams().at(1).get_points();


    //set and stile win message depending on the winner
    if (final_score_Phoenix > final_score_Dragon) {
        ui->win_message->setText("Phoenix Wins!");
        ui->win_message->setStyleSheet("QLabel {color: #3465a4; qproperty-alignment: 'AlignCenter'; font: 16pt}");
    } else if (final_score_Phoenix < final_score_Dragon) {
        ui->win_message->setText("Dragon Wins");
        ui->win_message->setStyleSheet("QLabel {color: #cc0000; qproperty-alignment: 'AlignCenter'; font: 16pt}");
    } else {
        ui->win_message->setText("Tie");
        ui->win_message->setStyleSheet("QLabel {color: black; qproperty-alignment: 'AlignCenter'; font: 16pt}");
    }

    ui->phoenix_points->setText(QString::number(final_score_Phoenix));
    ui->phoenix_points->setStyleSheet("QLabel {color: #3465a4; qproperty-alignment: 'AlignCenter'; font: 16pt}");
    ui->dragon_points->setText(QString::number(final_score_Dragon));
    ui->dragon_points->setStyleSheet("QLabel {color: #cc0000; qproperty-alignment: 'AlignCenter'; font: 16pt}");

    // Clear existing table
    ui->scoreTable->clearContents();
    ui->scoreTable->setRowCount(0);

    // Determine the number of rounds
    int maxRounds = pointTableA.size();

    // Add columns for pointTableA, pointTableB, and round number
    ui->scoreTable->setColumnCount(3);
    ui->scoreTable->setColumnWidth(0, 50);
    ui->scoreTable->setColumnWidth(1, 100);
    ui->scoreTable->setColumnWidth(2, 100);

    //set table to expand in height up to a certain number of rows. After that scrollbar appears
    ui->scoreTable->setMinimumHeight(30*std::min(maxRounds+1,15));
    ui->scoreTable->setMinimumWidth(300);
    ui->scoreTable->setShowGrid(false);
    ui->scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->scoreTable->verticalHeader()->setSectionsClickable(false);
    ui->scoreTable->setSelectionBehavior(QAbstractItemView::SelectRows);



    //table styling
    ui->scoreTable->setStyleSheet("QHeaderView::section { background-color: #efefef; }"
                                  "QScrollBar { background-color: transparent; width: 10px; }"
                                  "QTableView { border: none; background-color: transparent; }"
                                  "QScrollBar::handle { background-color: #cbcece; border-radius: 5px; max-height: 5px; }"
                                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { border: none; background-color: #888888; width: 2px; }"
                                  "QTableView::item { border: none; }"
                                  );

    QStringList headers;
    headers << "Round" << "Dragon"<<"Phoenix";
    ui->scoreTable->setHorizontalHeaderLabels(headers);


// After setting the headers, get the header item and change its color
    QTableWidgetItem* phoenix_header = ui->scoreTable->horizontalHeaderItem(1);
    QTableWidgetItem* dragon_header = ui->scoreTable->horizontalHeaderItem(2);


    // Add rows with point values and round numbers
    for (int round = 0; round < maxRounds; ++round) {
        ui->scoreTable->insertRow(round);

        // Add points from pointTableA
        if (round < pointTableA.size()) {
            QTableWidgetItem* itemA = new QTableWidgetItem(QString::number(pointTableA[round].get_value()));
            ui->scoreTable->setItem(round, 1, itemA);
        }

        // Add points from pointTableB
        if (round < pointTableB.size()) {
            QTableWidgetItem* itemB = new QTableWidgetItem(QString::number(pointTableB[round].get_value()));
            ui->scoreTable->setItem(round, 2, itemB);
        }

        // Add round number
        QTableWidgetItem* roundItem = new QTableWidgetItem(QString::number(round + 1));
        ui->scoreTable->setItem(round, 0, roundItem);
    }


    // If the point tables are empty, fill them with dummy data
    if (pointTableA.size() == 0) {

        for (int i = 1; i <= 10; i++) {
            pointTableA.push_back(serializable_value<int>(i));
            pointTableB.push_back(serializable_value<int>(i * 5));
        }
    }

    // Set alignment and color for all items in the table
    for (int i=0; i<3; i++) {
        for(int j=0; j<ui->scoreTable->rowCount(); j++) {
            QTableWidgetItem* item = ui->scoreTable->item(j, i);
            if (item)
                item->setTextAlignment(Qt::AlignCenter);
                item->setForeground(QColor(colors[i]));
                item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        }
    }
}



EndGameView::~EndGameView() {
    delete ui;
}

bool EndGameView::isShown(){
    return _isShown;
}

void EndGameView::setIsShown(bool isShown){
    _isShown = isShown;
}

void EndGameView::on_newGameButton_clicked()
{
    qDebug() << "Start new Game Request sent";
    GameController::newGame();
}


