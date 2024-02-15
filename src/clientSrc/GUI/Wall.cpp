#include "Wall.hpp"

Wall::Wall(sf::Vector2f pos, sf::Vector2f taille_case, sf::RenderWindow *window, std::pair<std::pair<int, int>, std::pair<int, int>> case_environs, bool hor) : position(pos), taille_case(taille_case), window(window), case_autour(case_environs), horizontal(hor)
{
    sf::Vector2f taille_mur;
    if (horizontal)
    {
        texture_mur.loadFromFile("ressources/mur_horizontal.png");
        taille_mur = sf::Vector2f((taille_case.x - 10) / 32, (taille_case.x - 10) / 32);
    }
    else
    {
        texture_mur.loadFromFile("ressources/mur_vertical.png");
        taille_mur = sf::Vector2f((taille_case.y - 10) / 32, (taille_case.y - 10) / 32);
    }
    sprite_mur.setTexture(texture_mur);
    sprite_mur.setPosition(pos.x + 5, pos.y + 5);
    sprite_mur.scale(taille_mur);
    couleur_sprite = sprite_mur.getColor();

    rect.setSize(sf::Vector2f(taille_case.x - 10, taille_case.y - 10));
    rect.setPosition(pos.x + 5, pos.y + 5);
    rect.setFillColor(sf::Color::Transparent);
}

Wall::~Wall()
{
}

bool Wall::contains(sf::Vector2i p)
{
    return p.x >= position.x &&
           p.x < position.x + taille_case.x &&
           p.y >= position.y &&
           p.y < position.y + taille_case.y;
}

void Wall::draw()
{
    // window->draw(rect);
    if (afficher)
        window->draw(sprite_mur);
    if (survole)
        window->draw(sprite_mur);
}

sf::Vector2f Wall::getPosition()
{
    return position;
}

void Wall::toggleAfficher()
{
    afficher = !afficher;
    if (afficher)
    {
        rect.setFillColor(sf::Color::Red);
    }
    else
    {
        rect.setFillColor(sf::Color::Transparent);
    }
}

void Wall::mouseClick(sf::Vector2i p)
{
}

bool Wall::isMur(std::pair<int, int> case1, std::pair<int, int> case2)
{
    if ((case1.first == case_autour.first.first && case1.second == case_autour.first.second) || (case1.first == case_autour.second.first && case1.second == case_autour.second.second))
    {
        if ((case2.first == case_autour.first.first && case2.second == case_autour.first.second) || (case2.first == case_autour.second.first && case2.second == case_autour.second.second))
        {
            return true;
        }
    }
    return false;
}

bool Wall::mouseMove(sf::Vector2i p, bool bombe)
{
    if (!afficher)
    {
        if (contains(p))
        {
            couleur_sprite.a = 150;
            sprite_mur.setColor(couleur_sprite);
            survole = true;
            return true;
        }
        else
        {
            couleur_sprite.a = 255;
            sprite_mur.setColor(couleur_sprite);
            survole = false;
            return false;
        }
    }
    else
    {
        if (bombe)
        {
            if (contains(p))
            {
                couleur_sprite.a = 0;
                sprite_mur.setColor(couleur_sprite);
            }
            else
            {
                couleur_sprite.a = 255;
                sprite_mur.setColor(couleur_sprite);
            }
        }
    }
    return false;
}