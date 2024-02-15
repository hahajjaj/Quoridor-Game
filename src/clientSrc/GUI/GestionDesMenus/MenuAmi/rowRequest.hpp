#ifndef __ROWREQUEST_HPP
#define __ROWREQUEST_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Button.hpp"

class rowRequest
{
private:
    float echelle;
    sf::Text nom_ami;
    sf::Font *font_jeu;
    sf::Vector2f position;
    int sizeChar;
    bool two_button;
    sf::Vector2f bouton_size;

    std::vector<Button*> boutons;
    sf::Texture bouton;
    sf::Texture bouton_selec;

    sf::Texture bouton_v;
    sf::Texture bouton_v_selec;
    
public:
    rowRequest(float echelle, std::string nom_ami, sf::Font *font_jeu, sf::Vector2f position, int sizeChar = 25 ,bool two_buttons = false, sf::Vector2f size = {50, 50});
    ~rowRequest();
    void loadRess();
    void setPosition(sf::Vector2f pos);
    void drawTo(sf::RenderWindow &fenetre);
    bool mouseMove(sf::Vector2i pos);
    bool mouseClick(sf::Vector2i pos, int &ecran_actuel);
    std::string getNomAmi(){return nom_ami.getString();}
    void setTexture(sf::Texture *text, sf::Texture *text2, sf::Vector2f size, std::string new_text);
    std::vector<Button*> getBoutons(){return boutons;}
};



#endif