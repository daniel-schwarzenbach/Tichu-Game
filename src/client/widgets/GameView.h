#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLabel>

#include "../../common/game_state/GameState.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameView; }
QT_END_NAMESPACE

class GameView : public QWidget
{
    Q_OBJECT

public:
    GameView(QWidget *parent = nullptr);
    ~GameView();

    void buildGameState(GameState* gameState, Player* me);

    bool isShown();
    void setIsShown(bool isShown);
    void updateSelectedCards();

private:
    Ui::GameView *ui;

    void buildPlayerHand(GameState* gameState, Player* me);
    void buildCurrentStack(GameState* gameState, Player* me);
    void buildOtherPlayers(GameState* gameState, Player* me);
    void buildTurnIndicator(GameState* gameState, Player* me);

    void generateNewGraphicsScene(QGraphicsScene** graphicsScene, QGraphicsView* graphicsView);

    constexpr static const QSize _card_size = QSize(80, 124);
    QGraphicsScene* _handScene;
    QGraphicsScene* _currentStackScene;
    QGraphicsScene* _cardsPlayer1;
    QGraphicsScene* _cardsPlayer2;
    QGraphicsScene* _cardsPlayer3;

    std::vector<const QGraphicsScene*> _playerCardViews;

    bool _isShown;

protected:
    void resizeEvent(QResizeEvent*) override;

private slots:
    void on_tichuButton_clicked();
    void on_passButton_clicked();
    void on_playButton_clicked();
    void on_bombButton_clicked();
    void on_chatButton_clicked();
    void on_deselectButton_clicked();
};
#endif // GAMEVIEW_H
