#ifndef __ROWFRIEND_HPP
#define __ROWFRIEND_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Button.hpp"

class rowFriend
{
private:
    float echelle;
    sf::Text nom_ami;
    sf::Font *font_jeu;
    sf::Vector2f position;

    std::vector<Button*> boutons;
    sf::Texture bouton;
    sf::Texture bouton_selec;
    sf::Texture bouton_chat;
    
    sf::Text nom_bouton_chat;
public:
    rowFriend(float echelle, std::string nom_ami, sf::Font *font_jeu, sf::Vector2f position);
    ~rowFriend();
    void loadRess();
    void setPosition(sf::Vector2f pos);
    void drawTo(sf::RenderWindow &fenetre);
    bool mouseMove(sf::Vector2i pos);
    bool mouseClick(sf::Vector2i pos, int &ecran_actuel);
    std::vector<Button*> getBoutons(){return boutons;}
    std::string getNomAmi(){return nom_ami.getString();}
};



#endif