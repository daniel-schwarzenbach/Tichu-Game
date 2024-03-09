#include "CardItem.h"
#include <QDebug>
#include "../../common/exceptions/TichuException.h"

CardItem::CardItem(int type, int family, bool isSelectable, bool cardSwap):
    _family(family),
    _type(type),
    _selectable(isSelectable),
    _cardSwap(cardSwap)
{
    if(family > 4 || family < 1){
        throw TichuException("int family must be between 1 and 4!");
    }

    if((type > 15 || type < 2) && !(type == 1 && family == 1)){
        throw TichuException("int type must be between 2 and 15!");
    }
}

CardItem::~CardItem()
{

}

int CardItem::get_type(){
    return _type;
}

int CardItem::get_family(){
    return _family;
}

void CardItem::select(){
    _selected = true;
}

void CardItem::unselect(){
    _selected = false;
}

bool CardItem::isSelected(){
    return _selected;
}

bool CardItem::cardSwap(){
    return _cardSwap;
}

bool CardItem::isSelectable(){
    return _selectable;
}
