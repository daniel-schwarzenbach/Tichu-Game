#ifndef CARDITEM_H
#define CARDITEM_H

#include <QGraphicsItem>

namespace Ui {
class CardItem;
}

class CardItem : public QGraphicsPixmapItem
{

public:
    explicit CardItem(int type = 0, int family = 0, bool isSelectable = false, bool cardSwap=false);
    ~CardItem();

    int get_family();
    int get_type();
    void select();
    void unselect();
    bool isSelected();
    bool isSelectable();
    bool cardSwap();

private:
    int _family;
    int _type;
    bool _selectable;
    bool _selected = false;
    bool _cardSwap=false;
};

#endif // CARDITEM_H
