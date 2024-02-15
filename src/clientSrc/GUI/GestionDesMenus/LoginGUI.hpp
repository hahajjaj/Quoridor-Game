#ifndef __LOGINGUI_H__
#define __LOGINGUI_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../Client.hpp"
#include "MenuAmi/menu_ami.hpp"


#include "Button.hpp"
#include "textbox.hpp"

class GUI;

class LoginGUI
{
private:
    int echelle;
    int width, height;
    sf::Font *font_jeu;
    Client *client;
    unique_ptr<MenuAmi>& menu_ami;
    GUI *interface;

    std::vector<Button*> boutons;

    Textbox *username;
    Textbox *password;

    sf::Texture cadre_petit;
    sf::Texture cadre_grand;
    sf::Texture bouton;
    sf::Texture bouton_selec;
    sf::Texture bouton_2;
    sf::Texture bouton_2_selec;
    sf::Texture bar_text;

    bool Enter = false;

    sf::Sprite sprite_cadre_petit;
    sf::Sprite sprite_cadre_grand;

    Button *bouton_connection;
    Button *bouton_inscription;
    Button *bouton_login;
    Button *bouton_signIn;
    Button *bouton_retour;


    bool ecran_login = true;
    bool mode_login = false;


    
public:
    LoginGUI(float echelle,int width, int height, sf::Font *font_jeu, Client *client, unique_ptr<MenuAmi> & menu_ami, GUI *interface);
    ~LoginGUI();
    void loadSprite();
    void loadButtons();
    void setPosition();
    void drawTo(sf::RenderWindow &fenetre);
    void mouseMove(sf::Vector2i pos);
    void mouseClick(sf::Vector2i pos, int &ecran_actuel);
    void textEntre(sf::Event evenement, int &ecran_actuel);
};





#endif // __LOGINGUI_H__