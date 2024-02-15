#ifndef __ITEM_HPP
#define __ITEM_HPP


#include <string>
#include "../../Jeu/header/Position.hpp"

class Item
{
private:
    std::string item;
    Point<> *p = nullptr;
public:
    Item(std::string nom_item);
    ~Item();
    std::string getitem();
    bool isItem(std::string act);
    void setPosition(Point<> *p);
    Point<>* getPosition(){return p;}
};

#endif