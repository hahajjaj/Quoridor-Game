#ifndef __MUR_HPP
#define __MUR_HPP

#include "../Position.hpp"
#include <vector>

class Mur{
private:
    std::pair<Point<>, Point<>> pos_mur;
public:
    Mur(Point<> pos1, Point<> pos2);
    ~Mur();
    std::pair<Point<>, Point<>> getPositionMur();
};

#endif