#include "Item.hpp"


Item::Item(float echelle, sf::RenderWindow *window, sf::Vector2f p, char type, std::vector<sf::Texture> *texture) : echelle(echelle), window(window), position(p), type(type), texture(texture)
{
        // std::cout << "position : " << position.x << " " << position.y << std::endl;
        // std::cout << type << std::endl;
    update(position);
    
    loadSprite();
}

Item::~Item()
{
    
}

sf::Vector2f Item::getPosition()
{
    return position;
}

void Item::setPosition(sf::Vector2f p)
{
    position = p;
}

void Item::draw(sf::Time frameTime)
{
    window->draw(sprite);
}

void Item::loadSprite()
{
    if(type == 't'){
    sprite.setTexture(texture->operator[](0));

    }else if (type == 's'){
    sprite.setTexture(texture->operator[](1));

    }
    sprite.scale((echelle * 55.0)/32.0, (echelle * 55.0)/32.0);
}

void Item::update(sf::Vector2f new_p)
{
    setPosition(sf::Vector2f((new_p.x*(echelle * 100))+((echelle * 20.0)/2)+(echelle * 10), (new_p.y*(echelle * 100))+((echelle * 45.0)/2)+10));
    sprite.setPosition(position);
    // animatedSprite->setPosition(sf::Vector2f(new_p.y*(echelle * 100)+((echelle * 45.0)/2), new_p.x*(echelle * 100)+((echelle * 20.0)/2)));
}
