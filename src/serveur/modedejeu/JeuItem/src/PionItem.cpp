#include "../header/PionItem.hpp"

PionItem::PionItem(int x, int y, int numeroJoueur):Pion(x,y,numeroJoueur){}

PionItem::~PionItem(){}

void PionItem::addItem(Item*i){
    items.push_back(i);
}

bool PionItem::getItem(std::string act){
    for(auto &i: items){
        if(act == i->getitem()){
            return true;
        }
    }
    return false;
}

void PionItem::supprimerItem(std::string act){
    for(auto &i: items)
        if(i->getitem() == act)
            items.erase(find(items.begin(), items.end(), i));
}