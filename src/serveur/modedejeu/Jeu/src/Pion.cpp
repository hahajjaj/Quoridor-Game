#include "../header/elements/Pion.hpp"

Pion::Pion(int x, int y, int numerojoueur):pos{x,y},numeroJoueur{numerojoueur},pos_initial{x,y}{
    nbMurs = 10;
}
Pion::~Pion(){
    
}

int Pion::getNumerojoueur(){
    return numeroJoueur;
}

void Pion::setPosition(int x, int y){
    pos.x = x;
    pos.y = y;
}

void Pion::setNbrMurs(int nbrMur){
    nbMurs = nbrMur;
}

int Pion::getNbrMurs(){
    return nbMurs;
}