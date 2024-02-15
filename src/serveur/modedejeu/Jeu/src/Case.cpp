#include "../header/Case.hpp"

Case::Case(int x, int y): pos{x,y}{
    pion = nullptr;
}

Case::~Case(){
    delete pion;
    pion = nullptr;
}

void Case::viderCase(){
    pion = nullptr;
}

Point<> Case::getPosition(){
    return pos;
}

Pion* Case::getPionCase(){
    return pion;
}

void Case::setPionCase(Pion *new_pion){
    pion = new_pion;
}

void Case::setVisited(int i){
    visited = i;
}