#ifndef _CASEITEM__HPP
#define _CASEITEM__HPP

#include "../../Jeu/header/Case.hpp"
#include "Item.hpp"
#include "PionItem.hpp"
#include <string>

class CaseItem : public Case
{
private:
    Item *item = nullptr;
public:
    CaseItem(int x, int y);
    ~CaseItem();
    PionItem* getPionCase(){return dynamic_cast<PionItem*>(pion);}
    Item* getItem(){return item;}
    bool isItem(){return(item != nullptr);}
    void setItem(Item *i);
};

#endif