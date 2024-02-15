#ifndef __MENUCLASSEMENT_H__
#define __MENUCLASSEMENT_H__

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Button.hpp"
#include "../../../Client.hpp"
#include "../Menu.hpp"

class GUI;
class MenuClassement
{
private:
    float echelle;
    int width, height;
    sf::Font *font_jeu;
    sf::Vector2f position;
    Client *client;
    GUI *interface;

    std::vector<Button*> bouton_generals;
    std::vector<Button*> boutons_onglets;

    std::vector<std::string> genre_classement{"cg", "cf", "ch"};

    int page_onglet = 0;

    int page_classement = 0;

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

    std::vector<std::string> ligne_classement;
    std::vector<sf::Text> TextLigne_classement;
    std::vector<std::vector<std::string>> lignes;

    sf::Sprite spriteCadreMenu;
    sf::Sprite spriteTableau;
    sf::Sprite spriteSousTableauXL;

    float ecart = 0;
    sf::Text rank, pseudo, score;

public:
    MenuClassement(float echelle, int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, GUI *interface);
    ~MenuClassement();
    void loadSprite();
    void loadButtons();
    void setPosition();
    void drawTo(sf::RenderWindow &fenetre);
    void mouseMove(sf::Vector2i pos);
    void mouseClick(sf::Vector2i pos, int &ecran_actuel);
    void parseRowClassement();
    void parseRowPosition(std::string s, int indice);
    void parseMessageClient(std::string s, std::vector<std::string> *liste);
    void update();
};


#endif // __MENUCLASSEMENT_H__