#ifndef TEAMVIEW_H
#define TEAMVIEW_H

#include <QWidget>
#include <QLabel>

#include "../../common/game_state/GameState.h"

namespace Ui {
class layout;
}

class TeamView : public QWidget
{
    Q_OBJECT

public:
    explicit TeamView(QWidget *parent = nullptr);
    ~TeamView();

    void buildGameState(GameState* gameState, Player* me);

    bool isShown();
    void setIsShown(bool isShown);

private slots:
    void on_LeftButton_clicked();

    void on_RightButton_clicked();

    void on_startButton_clicked();

private:
    Ui::layout *ui;

    std::vector<QLabel*> _Middle;
    std::vector<QLabel*> _Left;
    std::vector<QLabel*> _Right;

    std::string position = "middle";

    Player _me;

    bool _isShown;

    int _bluecount;
    int _redcount;
};

#endif // TEAMVIEW_H
