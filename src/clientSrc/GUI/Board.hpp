#ifndef __BOARD_HPP
#define __BOARD_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Case.hpp"
#include "Wall.hpp"
#include "Pion.hpp"
#include "Item.hpp"
#include "GestionDesMenus/Menu.hpp"
#include "../../common/common.hpp"
#include "../Client.hpp"
#include "Animation/AnimatedSprite.hpp"

class GUI;

enum etatAnimation{
    teleport1,
    teleport2,
    explosion
};

class BoardGui
{
private:
    float echelle;
    sf::RenderWindow *window;
    const int nombre_joueur;
    sf::Font *font_jeu;
    sf::Texture *cadre_menu;
    Client *client;
    GUI *interface;

    
    int joueur_client;
    int currentJoueur_serveur;

    etatAnimation enum_animation = teleport1;

    int last_indice = 0;

    float hauteur = 900;
    float largeur = 900;
    float hauteur_window;
    float largeur_window;
    const float taille_plateau = 9;
    sf::Vector2f taille_case;
    bool click_case = false;
    std::vector<std::vector<Case*>> plateau;
    std::vector<Pion*> vecteur_pions;
    std::vector<Item*> vecteur_items;
    std::vector<Wall*> vecteur_murs;
    sf::Texture texture_map;
    sf::Texture item1;
    sf::Texture item2;
    std::vector<sf::Texture> textures_items;
    sf::Sprite map;
    sf::Sprite eclair;
    std::string coup = "gm";

    bool mode_item = false;
    bool bombe = false;
    bool traverser = false;

    bool bombe_is_click = false;
    bool anime_bomb = false;
    
    bool pause = false;
    bool isFinished = false;
    bool popUpTournoi = false;

    int finish = 0;



    std::string ancien_etat;
    std::string nouvel_etat;

    std::vector<sf::Text> nombre_murs;

    // bouton de pause
    Button *bouton_de_pause;
    Button *bouton_retour;
    Button *bouton_bombe;
    sf::Texture texture_bouton_pause;
    sf::Texture texture_bouton_pause_selec;
    sf::Texture bouton_bomb;
    sf::Texture bouton_bomb_selec;

    sf::Texture bouton;
    sf::Texture bouton_selec;
    sf::Texture tunder1;
    sf::Texture tunder2;
    sf::Texture explosion_texture;

    // menu pause
    MenuGui *menu_pause;


    float timePopUp = 0;

    bool isWaitingOpponent = false;

    AnimatedSprite *animatedBombe;
    AnimatedSprite *animatedSprite;
    AnimatedSprite *animatedTunder;
    Animation *teleporte1;
    Animation *teleporte2;
    Animation *explosion;
    Animation *itemEclair;
    Animation *currentAnimation;

    bool anime = false;

public:
    BoardGui(float echelle, sf::RenderWindow *windown, int nombre_joueur, sf::Font * font, sf::Texture *cadre, int &ecran_actuel, Client *client, GUI *interface);
    ~BoardGui();
    void loadWall();
    void resetWallDraw();
    void initPion();
    void loadTextures();
    Wall* getMurVoisin(Wall* w);
    bool respecteDelimitation(int y, int x);
    void setPositionAnimation(sf::Vector2f pos);
    std::vector<Case*> getNeighbour(Point<> pos_init);
    void updtate(sf::Vector2i position, int &ecran_actuel);
    void mouseMove(sf::Vector2i p);
    void drawBomb(sf::Time frameTime);
    void loadBoard();
    void draw(sf::Time frameTime);
    void updateBoard(std::string etatPlateau);
    void loadMenuPause(int &ecran_actuel);
    void setPositionPion(std::string mouv);

};

#endif