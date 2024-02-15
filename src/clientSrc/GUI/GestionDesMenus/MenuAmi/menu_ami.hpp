#ifndef __MENU_AMI_HPP
#define __MENU_AMI_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Button.hpp"
#include "../textbox.hpp"
#include "rowFriend.hpp"
#include "rowRequest.hpp"
#include "../../../Client.hpp"
#include "../MenuChat/menuChat.hpp"

class MenuAmi
{
private:

    float echelle;
    int width, height;
    sf::Font *font_jeu;
    sf::Vector2f position;

    Client *client;
    unique_ptr<MenuChat>& menu_chat;

    std::vector<Button*> boutons;
    int page = 0;

    Textbox *search_friend;

    sf::Texture cadre_menu;
    sf::Texture bouton_x;
    sf::Texture bouton_x_selec;
    sf::Texture bouton_v;
    sf::Texture bouton_v_selec;
    sf::Texture chat;
    sf::Texture bouton_next;
    sf::Texture bouton_previous;
    sf::Texture bouton_next_selec;
    sf::Texture bouton_previous_selec;
    sf::Texture search;
    sf::Texture tableau1;
    sf::Texture tableau2;
    sf::Texture bouton_simple;
    sf::Texture bouton_simple_selec;

    float timePopUp;

    // pour l'affichage des amis et des demandes
    std::vector<rowFriend*> liste_ami_row;
    std::vector<rowRequest*> liste_demandes_row;

    sf::Text text_tableau_1;
    sf::Text text_tableau_2;

    sf::Sprite spriteCadreMenu;
    sf::Sprite spriteChat;
    sf::Sprite spriteSearch;
    sf::Sprite spriteTableau1;
    sf::Sprite spriteTableau2;

    std::vector<std::string> friendlist;
    std::vector<std::string> requestList;

public:
    MenuAmi(float echelle, int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, unique_ptr<MenuChat>& menu_chat);
    ~MenuAmi();
    void update(std::string liste_ami, std::string liste_invitations);
    void loadSprite();
    void loadButtons();
    void setPosition();
    void drawTo(sf::RenderWindow &fenetre, sf::Time frameTime);
    void mouseMove(sf::Vector2i pos);
    void mouseClick(sf::Vector2i pos, int &ecran_actuel);
    void textEntre(sf::Event evenement);
    void update();
    void updateFriendList();
    void parseListeAmi(std::string s);
    void updateRequestList();
    void parseRequestList(std::string s);
};


#endif