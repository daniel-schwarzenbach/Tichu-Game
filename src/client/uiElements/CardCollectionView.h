#ifndef CARDCOLLECTIONVIEW_H
#define CARDCOLLECTIONVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include "CardItem.h"

#include "../../common/game_state/GameState.h"

namespace Ui {
class CardCollectionView;
}

class CardCollectionView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CardCollectionView(QWidget *parent = nullptr);
    ~CardCollectionView();
    void updateSelectedCards();
    int getNSelectedCards();

private:
    Ui::CardCollectionView *ui;
    int n_selectedCards;

protected:
    void mousePressEvent(QMouseEvent * event);
};

#endif // CARDCOLLECTIONVIEW_H
