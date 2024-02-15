#ifndef __PION_HPP
#define __PION_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Animation/AnimatedSprite.hpp"

class Pion
{
private:
    float echelle;
    sf::RenderWindow *window;
    sf::Vector2f position;
    int id;
    
    sf::RectangleShape rect;
    sf::Texture texture;
    sf::Texture texture2;
    sf::Sprite sprite;
    bool firstLaunch = true;
    bool afficher = true;


    // pour animation du sprite
    AnimatedSprite *animatedSprite;
    Animation *idle;
    Animation *wakeUp;
public:
    Pion(float echelle, sf::RenderWindow *window, sf::Vector2f p, int id);
    ~Pion();
    sf::Vector2f getPosition();
    void setAfficher(bool var){afficher = var;}
    bool getAfficher(){return afficher;}
    int getId(){return id;}
    void setPosition(sf::Vector2f p);
    sf::IntRect getAnimationSize(){return idle->getFrame(0);}
    void draw(sf::Time frameTime);
    void loadSprite();
    void update(sf::Vector2f new_p);

};

#endif