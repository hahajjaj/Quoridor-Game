#ifndef __ITEM_H__
#define __ITEM_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Animation/AnimatedSprite.hpp"

class Item
{
private:
    float echelle;
    sf::RenderWindow *window;
    sf::Vector2f position;
    char type;
    std::vector<sf::Texture> *texture;

    sf::Sprite sprite;
    
    // pour les animation de sprite
    AnimatedSprite *animatedSprite;
    Animation *idle;
public:

    Item(float echelle, sf::RenderWindow *window, sf::Vector2f p, char type, std::vector<sf::Texture> *texture);
    ~Item();
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f p);
    void draw(sf::Time frameTime);
    void loadSprite();
    void update(sf::Vector2f new_p);
    
};

#endif // __ITEM_H__