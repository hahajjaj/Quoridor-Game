#ifndef __PIONITEM_HPP
#define __PIONITEM_HPP

#include "../../Jeu/header/elements/Pion.hpp"
#include "Item.hpp"

class PionItem: public Pion
{
private:
    vector<Item* > items;
public:
    PionItem(int x, int y, int numerojoueur);
    ~PionItem();

    bool isItem(){return(!items.empty());}
    void supprimerItem(std::string act);

    void addItem(Item* i);
    bool getItem(std::string act);
    vector<Item*> getItems(){return items;}


};

#endif