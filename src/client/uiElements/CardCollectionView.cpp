#include "CardCollectionView.h"

#include <QMouseEvent>
#include "../../common/game_state/GameState.h"
#include "../GameController.h"

CardCollectionView::CardCollectionView(QWidget *parent) :
    QGraphicsView(parent),
    n_selectedCards(0)
{

}

CardCollectionView::~CardCollectionView()
{

}

void CardCollectionView::mousePressEvent(QMouseEvent * event){
    CardItem * pressedItem;

    QPoint pressPoint = event->pos();

    if(scene())
    {
        if (!items().isEmpty())
        {
            QPointF scenePoint = mapToScene(event->pos()) ;
            QGraphicsItem * item = scene()->itemAt(scenePoint, transform());

            if(pressedItem = dynamic_cast<CardItem*>(item))
            {

                //Checks wether CardCollectionView displays CurrentStack or Hand
                if(pressedItem->isSelectable()){

                //code for item press, instead of using QGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
                    if(pressedItem->isSelected() == false) {
                        int family = pressedItem->get_family();
                        int type = pressedItem->get_type();

                        //Adds Card to Card Collection
                        Card card = Card(type, family);
                        if (pressedItem->cardSwap() == false) {
                            GameController::_cardSelection->add_card(card);
                        } else {
                            GameController::_cardSelection->remove_all_cards();
                            GameController::_cardSelection->add_card(card);
                        }

                        GameController::updateSelectedCards();
                    }
                    else{
                        int family = pressedItem->get_family();
                        int type = pressedItem->get_type();

                        //Removes Card from Card Collection
                        Card card = Card(type, family);
                        GameController::_cardSelection->remove_card(card);

                        GameController::updateSelectedCards();
                    }
                }
            }
        }
    }
}

void CardCollectionView::updateSelectedCards(){
    qDebug() << "Selcted are updated";
    n_selectedCards = 0;

    for(int i = 0; i< items().length(); ++i){
        CardItem *current_item = dynamic_cast<CardItem*>(items().at(i));
        Card current_card = Card(current_item->get_type(), current_item->get_family());

        if(GameController::_cardSelection->try_get_card(current_card)){
            n_selectedCards++;
            if(!current_item->isSelected()){
                current_item->select();
                current_item->moveBy(0,-100);
            }
        }else{
            if(current_item->isSelected()){
                current_item->unselect();
                current_item->moveBy(0,100);
            } 
        } 
    }
}

int CardCollectionView::getNSelectedCards(){
    return n_selectedCards;
}
