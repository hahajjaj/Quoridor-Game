#ifndef __PION_HPP
# define __PION_HPP

#include <string>
#include <iostream>
#include "../Position.hpp"

class Pion
{
protected:
    int nbMurs;
    Point<> pos;
    int numeroJoueur;
    Point<> pos_initial;
public:
    Pion(int x, int y, int numerojoueur);
    virtual ~Pion();

    virtual Point<> getPosition(){return pos;}
    virtual Point<> getPositionInitial(){return pos_initial;}
    virtual void setPosition(int x, int y);

    virtual void setNbrMurs(int nbrMur);
    virtual int getNbrMurs();

    virtual int getNumerojoueur();
};

#endif