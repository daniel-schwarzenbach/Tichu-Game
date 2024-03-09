#ifndef CARDSWAPVIEW_H
#define CARDSWAPVIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLabel>

#include "../../common/game_state/GameState.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CardSwapView; }
QT_END_NAMESPACE

class CardSwapView : public QWidget
{
    Q_OBJECT

public:
    CardSwapView(QWidget *parent = nullptr);
    ~CardSwapView();

    void buildGameState(GameState* gameState, Player* me);

    bool isShown();
    void setIsShown(bool isShown);

    void updateSelectedCards();

private:
    Ui::CardSwapView *ui;

    void buildPlayerHand(GameState* gameState, Player* me);
    void buildOtherPlayers(GameState* gameState, Player* me);

    void generateNewGraphicsScene(QGraphicsScene** graphicsScene, QGraphicsView* graphicsView);

    constexpr static const QSize _card_size = QSize(80, 124);
    QGraphicsScene* _handScene;
    QGraphicsScene* _cardsPlayer1;
    QGraphicsScene* _cardsPlayer2;
    QGraphicsScene* _cardsPlayer3;

    std::vector<const QGraphicsScene*> _playerCardViews;

    bool _isShown;
    std::vector<Card> _selectedCards;

    std::string _gamephase;

protected:
    void resizeEvent(QResizeEvent*) override;
private slots:
    void on_swapButton1_clicked();
    void on_swapButton2_clicked();
    void on_swapButton3_clicked();
    void on_ContinueButton_clicked();
    void on_TichuButton_clicked();
};
#endif // CARDSWAPVIEW_H
