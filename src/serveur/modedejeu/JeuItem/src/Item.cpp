#include "../header/Item.hpp"

Item::Item(std::string nom_item) : item{nom_item}
{
}

Item::~Item()
{
}

std::string Item::getitem()
{
    return item;
}

void Item::setPosition(Point<> *p)
{
    this->p = p;
}

bool Item::isItem(std::string act){
    if (act == item){
        return true;
    }
    return false;
}