#ifndef __WALL_HPP
#define __WALL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Wall
{
private:
    sf::Vector2f position;
    sf::Vector2f taille_case;
    sf::RenderWindow *window;
    std::pair<std::pair<int, int>, std::pair<int, int>> case_autour;
    bool horizontal;

    bool afficher = false;
    bool survole = false;
    sf::RectangleShape rect;
    sf::Texture texture_mur;
    sf::Sprite sprite_mur;
    sf::Color couleur_sprite;

public:
    Wall(sf::Vector2f pos, sf::Vector2f taille_case, sf::RenderWindow *window, std::pair<std::pair<int, int>, std::pair<int, int>> case_environs, bool hor);
    ~Wall();
    bool contains(sf::Vector2i p);
    void draw();
    sf::Vector2f getPosition();
    bool getAfficher(){return afficher;}
    void setAfficher(bool var){afficher = var;}
    void toggleAfficher();
    bool mouseMove(sf::Vector2i p, bool bombe);
    void mouseClick(sf::Vector2i p);
    bool getOrientation(){return horizontal;}
    std::pair<std::pair<int, int>, std::pair<int, int>> getCasesEnvironnante(){return case_autour;}
    bool isMur(std::pair<int, int> case1, std::pair<int, int> case2);
};

#endif