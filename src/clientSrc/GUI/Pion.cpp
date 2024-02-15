#include "Pion.hpp"

Pion::Pion(float echelle, sf::RenderWindow *window, sf::Vector2f p, int id) : echelle(echelle), window(window), position(p), id(id)
{
    loadSprite();
}

Pion::~Pion()
{
    delete animatedSprite;
    delete idle;
    delete wakeUp;
}

sf::Vector2f Pion::getPosition()
{
    return position;
}

void Pion::setPosition(sf::Vector2f p)
{
    position = p;
}

void Pion::draw(sf::Time frameTime)
{
    if (firstLaunch)
    {
        animatedSprite->play(*wakeUp);
        animatedSprite->update(frameTime);
        if (!animatedSprite->isPlaying())
        {
            firstLaunch = false;
        }
    }
    else
    {
        animatedSprite->setLooped(true);
        animatedSprite->play(*idle);
        animatedSprite->update(frameTime);
    }
    if (afficher)
    {
        window->draw(*animatedSprite);
    }
}

void Pion::loadSprite()
{
    switch (id)
    {
    case 0:
        texture.loadFromFile("ressources/idle_c_j.png");
        texture2.loadFromFile("ressources/wakeup_c_j.png");
        break;
    case 1:
        texture.loadFromFile("ressources/idle_c_v.png");
        texture2.loadFromFile("ressources/wakeup_c_v.png");
        break;
    case 2:
        texture.loadFromFile("ressources/idle_c_r.png");
        texture2.loadFromFile("ressources/wakeup_c_r.png");
        break;
    case 3:
        texture.loadFromFile("ressources/idle_c_b.png");
        texture2.loadFromFile("ressources/wakeup_c_b.png");
        break;
    }
    idle = new Animation();
    idle->setSpriteSheet(texture);
    idle->addFrame(sf::IntRect(0, 0, 32, 48));
    idle->addFrame(sf::IntRect(0, 48, 32, 48));
    idle->addFrame(sf::IntRect(0, 96, 32, 48));
    idle->addFrame(sf::IntRect(0, 144, 32, 48));
    idle->addFrame(sf::IntRect(0, 192, 32, 48));
    idle->addFrame(sf::IntRect(0, 240, 32, 48));
    idle->addFrame(sf::IntRect(0, 288, 32, 48));
    idle->addFrame(sf::IntRect(0, 336, 32, 48));

    wakeUp = new Animation();
    wakeUp->setSpriteSheet(texture2);
    wakeUp->addFrame(sf::IntRect(0, 448, 32, 32));
    wakeUp->addFrame(sf::IntRect(0, 416, 32, 32));
    wakeUp->addFrame(sf::IntRect(0, 384, 32, 32));
    wakeUp->addFrame(sf::IntRect(0, 352, 32, 32));
    wakeUp->addFrame(sf::IntRect(0, 320, 32, 32));
    wakeUp->addFrame(sf::IntRect(0, 288, 32, 32));
    wakeUp->addFrame(sf::IntRect(0, 240, 32, 48));
    wakeUp->addFrame(sf::IntRect(0, 192, 32, 48));
    wakeUp->addFrame(sf::IntRect(0, 144, 32, 48));
    wakeUp->addFrame(sf::IntRect(0, 96, 32, 48));
    wakeUp->addFrame(sf::IntRect(0, 48, 32, 48));
    wakeUp->addFrame(sf::IntRect(0, 0, 32, 48));

    animatedSprite = new AnimatedSprite(sf::seconds(0.1), true, false);
    animatedSprite->scale((echelle * 55.0) / 32.0, (echelle * 55.0) / 32.0);

    // sprite.setTexture(texture);
    // sprite.scale(sf::Vector2f(65.0/1240.0, 65.0/1754.0));
    // sprite.setPosition(position);
}

void Pion::update(sf::Vector2f new_p)
{
    setPosition(sf::Vector2f((new_p.x * (echelle * 100)) + ((echelle * 45.0) / 2) + 10, (new_p.y * (echelle * 100)) + ((echelle * 20.0) / 2) + (echelle * 10)));
    animatedSprite->setPosition(sf::Vector2f(new_p.x * (echelle * 100) + ((echelle * 45.0) / 2), new_p.y * (echelle * 100) + ((echelle * 20.0) / 2)));
}