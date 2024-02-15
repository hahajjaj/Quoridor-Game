#ifndef __MENUREGLEMENT_H__
#define __MENUREGLEMENT_H__

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Button.hpp"
#include "../../../Client.hpp"
#include "../Menu.hpp"

class GUI;

class MenuReglement
{
private:
    float echelle;
    int width, height;
    sf::Font *font_jeu;
    sf::Vector2f position;
    Client *client;
    GUI *interface;

    std::vector<Button*> bouton_generals;

    int page = 0;
    
    sf::Texture bouton_petit;
    sf::Texture bouton_petit_selec;
    sf::Texture cadre;
    sf::Texture next;
    sf::Texture next_selec;
    sf::Texture onglet;
    sf::Texture onglet_selec;
    sf::Texture previous;
    sf::Texture previous_selec;
    sf::Texture tableau;
    sf::Texture sousTableau_ecranSecondaires;

    sf::Text regles;
    sf::Text regles2;

    sf::Sprite spriteCadreMenu;
    sf::Sprite spriteTableau;
    sf::Sprite spriteSousTableauXL;

public:
    MenuReglement(float echelle, int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, GUI *interface);
    ~MenuReglement();
    void loadSprite();
    void loadButtons();
    void setPosition();
    void drawTo(sf::RenderWindow &fenetre);
    void mouseMove(sf::Vector2i pos);
    void mouseClick(sf::Vector2i pos, int &ecran_actuel);
    void update();
};

#endif // __MENUREGLEMENT_H__