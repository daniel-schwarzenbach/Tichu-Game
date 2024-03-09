//
// Created by dominic on 30.04.23.
//
/*
#ifndef TICHU_ENDROUNDVIEW_H
#define TICHU_ENDROUNDVIEW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class endroundview; }
QT_END_NAMESPACE

class endroundview : public QWidget {
Q_OBJECT

public:
    explicit endroundview(QWidget *parent = nullptr);

    ~endroundview() override;

private:
    Ui::endroundview *ui;
};


#endif //TICHU_ENDROUNDVIEW_H
*/
#ifndef ENDROUNDVIEW_H
#define ENDROUNDVIEW_H

#include <QWidget>
#include "../../common/game_state/GameState.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EndRoundView; }
QT_END_NAMESPACE

class EndRoundView : public QWidget
{
Q_OBJECT

public:
    EndRoundView(QWidget *parent = nullptr);
    ~EndRoundView();
    void updateGameState(GameState* gameState, Player* _me);

    bool isShown();
    void setIsShown(bool isShown);

private slots:
    void on_newRoundButton_clicked();
private:

    Ui::EndRoundView *ui;

    bool _isShown;

};
#endif // ENDROUNDVIEW_H
