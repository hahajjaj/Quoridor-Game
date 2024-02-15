#ifndef __CASE_HPP
#define __CASE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Animation/AnimatedSprite.hpp"

enum anim
{
    nothing,
    idle,
    open,
    closed
};

class Case
{
private:
    float echelle;
    sf::Vector2f position;
    sf::Vector2f taille_case;
    sf::RenderWindow *window;
    std::pair<int, int> position_plateau;
    std::string nom_case = "";
    sf::Texture *texture;

    anim etat_anim = nothing;
    sf::RectangleShape rect;
    bool click = false;

    // pour animation du sprite
    sf::Sprite sprite_portail;
    
    AnimatedSprite *animatedSprite;
    Animation *portal_final;
    Animation *currentAnimation;


    bool isOpening = false;
    bool isFerme = true;

public:
    Case(float echelle, sf::Vector2f pos, sf::Vector2f taille_case, sf::RenderWindow *window, std::pair<int, int> pos_plat, std::string nom_case, sf::Texture *texture);
    ~Case();
    bool contains(sf::Vector2i p);
    void loadSprite();
    void draw(sf::Time frameTime);
    void mouseMove(sf::Vector2i p);
    sf::Vector2f getPosition();
    void mouseClick(sf::Vector2i p);
    void setColor(sf::Color c);
    std::pair<int, int> getPosPlateau(){return position_plateau;}
    void setClick(bool var);
    bool getClick(){return click;}
    std::string getNomCase(){return nom_case;}
};

#endif