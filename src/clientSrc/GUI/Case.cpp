#include "Case.hpp"

Case::Case(float echelle, sf::Vector2f pos, sf::Vector2f taille_case, sf::RenderWindow *window, std::pair<int, int> pos_plat, std::string nom_case, sf::Texture *texture) : echelle(echelle), position(pos), taille_case(taille_case), window(window), position_plateau(pos_plat), nom_case(nom_case), texture(texture)
{
    rect.setSize(taille_case);
    rect.setPosition(pos);
    rect.setOutlineColor(sf::Color(255,255,255,10));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness((echelle * 4));
    loadSprite();
}

Case::~Case()
{
    delete animatedSprite;
    delete portal_final;
}

bool Case::contains(sf::Vector2i p)
{
    return p.x >= position.x &&
           p.x < position.x + taille_case.x &&
           p.y >= position.y &&
           p.y < position.y + taille_case.y;
}

void Case::draw(sf::Time frameTime)
{
    window->draw(rect);
    if(click){
        animatedSprite->play(*currentAnimation);
        animatedSprite->update(frameTime);
        window->draw(*animatedSprite);
    }
}

void Case::setColor(sf::Color c){
    rect.setOutlineColor(c);
}

void Case::loadSprite()
{
    portal_final = new Animation();
    portal_final->setSpriteSheet(*texture);
    portal_final->addFrame(sf::IntRect(0, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(48, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(96, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(144, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(192, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(240, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(288, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(336, 0, 48, 48));
    portal_final->addFrame(sf::IntRect(384, 0, 48, 48));

    animatedSprite = new AnimatedSprite(sf::seconds(0.1), true, true);
    currentAnimation = portal_final;
    animatedSprite->scale((echelle * 40.0) / 48.0, (echelle * 40.0) / 48.0);
    // animatedSprite->setRotation(90.f);
    animatedSprite->setPosition(sf::Vector2f(position.x + ((echelle * 35) / 2), position.y + ((echelle * 35) / 2)));
}

sf::Vector2f Case::getPosition()
{
    return position;
}

void Case::mouseClick(sf::Vector2i p)
{
    click = !click;
}

void Case::mouseMove(sf::Vector2i p)
{
    if (contains(p))
    {
        rect.setOutlineColor(sf::Color(106,235,255,150));
    }
    else
    {
        rect.setOutlineColor(sf::Color(255,255,255,10));
    }
}

void Case::setClick(bool var)
{
    click = var;
}