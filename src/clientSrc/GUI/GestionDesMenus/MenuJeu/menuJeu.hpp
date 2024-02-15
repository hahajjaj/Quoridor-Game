#ifndef __MENUJEU_H__
#define __MENUJEU_H__

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Button.hpp"
#include "../textbox.hpp"
#include "../MenuAmi/rowFriend.hpp"
#include "../MenuAmi/rowRequest.hpp"
#include "../../../Client.hpp"
#include "../Menu.hpp"

class GUI;
class MenuJeu
{
private:
    int width, height;
    sf::Font *font_jeu;
    sf::Vector2f position;
    Client *client;
    MenuGui *menu_principal;
    GUI *interface;

    std::vector<Button*> bouton_generals;
    std::vector<Button*> boutons1;
    std::vector<Button*> boutons_onglets;
    
    int page_amis = 0;
    int page_onglet = 0;
    int page_demande_partie = 0;
    int page_demande_sauvegarde = 0;



    bool initPartie = false;

    sf::Texture choix_mode;
    sf::Texture bouton_petit;
    sf::Texture bouton_petit_selec;
    sf::Texture cadre;
    sf::Texture next;
    sf::Texture next_selec;
    sf::Texture onglet;
    sf::Texture onglet_selec;
    sf::Texture previous;
    sf::Texture previous_selec;
    sf::Texture sous_tableau_grand;
    sf::Texture sous_tableau_petit;
    sf::Texture tableau;
    sf::Texture sousTableau_ecranSecondaires;

    sf::Sprite spriteCadreMenu;
    sf::Sprite spriteTableau;
    sf::Sprite spriteSousTableau_petit;
    sf::Sprite spriteSousTableau_grand;
    sf::Sprite spriteChoixMode;
    sf::Sprite spriteChoixJoueur;
    sf::Sprite spriteSousTableauXL;

    std::vector<std::string> mode_de_jeu{"Normal", "Item", "Tournoi"};
    int indice_mode_de_jeu = 0;

    std::vector<std::string> nombre_joueurs{"2 joueurs" ,"4 joueurs"};
    int indice_nombre_joueur = 0;

    sf::Text mode;
    sf::Text nbrJoueur;

    sf::Text nom_mode;
    sf::Text nom_nbrJoueur;

    sf::Text annonce;
    
    bool mode_tournoi = false;

    std::vector<rowRequest*> liste_demandes_row;
    std::vector<rowRequest*> liste_demandes_partie_row;
    std::vector<rowRequest*> liste_demandes_sauvegarde_row;

    std::vector<std::string> friendlist;
    std::vector<std::string> invitelist;
    std::vector<std::string> sauvegardelist;

    std::vector<std::string> joueurs_invite;

    float timePopUp = 0;

public:
    MenuJeu(int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, MenuGui &menu_principal, GUI *interface);
    ~MenuJeu();
    void loadSprite();
    void loadButtons();
    void setPosition();
    void setModeBoolTournoi(bool var){mode_tournoi = var;}
    void drawTo(sf::RenderWindow &fenetre, sf::Time frameTime);
    void mouseMove(sf::Vector2i pos);
    void mouseClick(sf::Vector2i pos, int &ecran_actuel);
    void parseConnectedFriends();
    void parseInvitation();
    void parseSaveInvit();
    void parseMessageClient(std::string s, std::vector<std::string> *liste);
    void update();
};


#endif // __MENUJEU_H__