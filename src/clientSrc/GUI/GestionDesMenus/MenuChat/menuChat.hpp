#ifndef __MENUCHAT_H__
#define __MENUCHAT_H__

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Button.hpp"
#include "../textbox.hpp"
#include "../../../Client.hpp"


class MenuChat
{
private:
    float echelle;
    int width, height;
    sf::Vector2f position;
    sf::Font *font_jeu;
    Client *client;

    std::vector<Button*> boutons;
    Textbox *message;

    sf::Texture cadre_menu;
    sf::Texture tableau;
    sf::Texture sousTableau1;
    sf::Texture sousTableau2;
    sf::Texture barre_chat;
    sf::Texture send_b;
    sf::Texture send_selec;
    sf::Texture bouton;
    sf::Texture bouton_selec;

    sf::Text nom_ami;

    sf::Sprite spriteCadreMenu;
    sf::Sprite spriteTableau;
    sf::Sprite spriteSousTableau1;
    sf::Sprite spriteSousTableau2;

    std::string ami = "";
    std::vector<std::string> chatlog;
    std::vector<sf::Text*> lines;

    bool refresh = false;


public:
    MenuChat(float echelle, int width, int height, sf::Vector2f position, sf::Font *font_jeu, Client *client);
    ~MenuChat();
    bool setCorrespondant(std::string nom);
    void initLines();
    void update(std::string s);
    void loadButtons();
    void loadSprite();
    void setPosition();
    void parseChat(std::string c);
    void drawTo(sf::RenderWindow &fenetre);
    void mouseMove(sf::Vector2i pos);
    void mouseClick(sf::Vector2i pos, int &ecran_actuel);
    void textEntre(sf::Event evenement);
};

#endif // __MENUCHAT_H__