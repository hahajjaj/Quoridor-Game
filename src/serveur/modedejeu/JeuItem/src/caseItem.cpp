#include "../header/caseItem.hpp"

CaseItem::CaseItem(int x, int y): Case(x,y){}

CaseItem::~CaseItem(){
    delete pion;
    pion = nullptr;
}

void CaseItem::setItem(Item *i){
    item = i;
}