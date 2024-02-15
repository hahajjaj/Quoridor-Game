#ifndef __INTERFACE_GUI_HPP
# define __INTERFACE_GUI_HPP

#include "../common/common.hpp"
#include "./Interface.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <SFML/Audio.hpp>
#include "GUI/Board.hpp"
#include "GUI/Animation/AnimatedSprite.hpp"
#include "GUI/GestionDesMenus/Menu.hpp"
#include "GUI/GestionDesMenus/MenuAmi/menu_ami.hpp"
#include "GUI/GestionDesMenus/LoginGUI.hpp"
#include "GUI/GestionDesMenus/MenuJeu/menuJeu.hpp"
#include "GUI/GestionDesMenus/MenuChat/menuChat.hpp"
#include "GUI/GestionDesMenus/MenuClassement/menuClassement.hpp"
#include "GUI/GestionDesMenus/MenuReglement/menuReglement.hpp"

class GUI: public Interface
{

private:
    int width = 1300;
    int height = 1000;

    sf::Clock clock;
    bool isPlaying = false;
    int ecran_actuel = 4;
    // ajout de la musique
    sf::SoundBuffer buffer_audio;
    sf::Sound music_fond;
    sf::Event evenement;
    unique_ptr<sf::RenderWindow> fenetre;
    sf::Font font;
    sf::Texture backgroundImage;
    sf::Sprite spriteBackgroundImage;
    sf::Texture cadre_menu;
    unique_ptr<BoardGui> plateau;
    std::vector<MenuGui*> menus;
    unique_ptr<MenuAmi> menu_ami;
    unique_ptr<LoginGUI> menu_login;
    unique_ptr<MenuJeu> menu_jeu;
    unique_ptr<MenuChat> menu_chat;
    unique_ptr<MenuClassement> menu_classement;
    unique_ptr<MenuReglement> menu_reglement;

    float echelle = 1;

    bool showMessage = false;
    bool limitShowMessage = false;
    bool initPartie = false;
    bool updateResolution = false;

    // pour pop up
    sf::Text annonce;
    sf::Texture cadre_popup;
    sf::Sprite spriteCadrePopup;


    void handler();
public:
    GUI(Client *client);
    ~GUI();
    void run() override;
    bool initBoard();
    void setResolution(float new_res);
    void getResolution();
    float getEchelle(){return echelle;}
    int getNombreJoueur(std::string etat);
    void setInitPartie(bool var){initPartie = var;}
    unique_ptr<sf::RenderWindow>& getFenetre(){return fenetre;}
    void showPopup(std::string message, bool limitePopup = false);
    void hidePopup();
    void setEcranActuel(int ecran){ecran_actuel = ecran;}
    void generateWindow();
};

#endif