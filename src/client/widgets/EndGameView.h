//
// Created by dominic on 16.05.23.
//

#ifndef TICHU_ENDGAMEVIEW_H
#define TICHU_ENDGAMEVIEW_H
#include "../../common/game_state/GameState.h"

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class EndGameView; }
QT_END_NAMESPACE

class EndGameView : public QWidget {
Q_OBJECT

public:
    explicit EndGameView(QWidget *parent = nullptr);
    bool isShown();
    void setIsShown(bool isShown);
    void updateGameState(GameState* gameState, Player* _me);

    ~EndGameView() override;
private slots:
    void on_newGameButton_clicked();
private:
    Ui::EndGameView *ui;
    bool _isShown;
};


#endif //TICHU_ENDGAMEVIEW_H
