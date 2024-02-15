#ifndef __CASE_HPP
# define __CASE_HPP

#include "elements/Pion.hpp"

class Case{
protected:
    Point<> pos;
    Pion *pion;
    
    int visited = 0;
public:
    Case(int x, int y);
    virtual ~Case();
    void viderCase();
    virtual Point<> getPosition();
    Pion* getPionCase();
    
    virtual void setPionCase(Pion *new_pion);
    virtual void setVisited(int i);
    virtual int getVisited(){return visited;}
};

#endif