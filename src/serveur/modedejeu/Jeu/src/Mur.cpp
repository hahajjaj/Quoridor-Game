#include "../header/elements/Mur.hpp"

Mur::Mur(Point<> pos1, Point<> pos2){
    pos_mur = std::make_pair(pos1,pos2);
}

Mur::~Mur(){

}

std::pair<Point<>, Point<>> Mur::getPositionMur(){
    return pos_mur;
}