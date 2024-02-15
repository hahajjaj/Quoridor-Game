#ifndef __MENU_HPP
#define __MENU_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Button.hpp"

class MenuGui
{
    float echelle;
    std::string nom_menu;
    int width, height;
    int ecran_actuel;
    std::vector<std::string> noms_boutons;
    sf::Vector2f position;
    sf::Font *font;
    sf::Texture *cadre_menu;
    
    std::vector<Button *> boutons;

    sf::Texture texture_bouton;
    sf::Texture texture_bouton_selec;
    sf::Sprite sprite_cadre;



public:
    MenuGui(float echelle, std::string nom_menu ,int width, int height, int &ecran_actuel, std::vector<std::string> noms_boutons, sf::Vector2f position, sf::Font *font, sf::Texture *cadre) : echelle(echelle), nom_menu(nom_menu), width(width), height(height),  ecran_actuel(ecran_actuel), noms_boutons(noms_boutons), position(position), font(font), cadre_menu(cadre)
    {
        sprite_cadre.setTexture(*cadre_menu);
        sprite_cadre.setPosition((echelle *position.x), (echelle *position.y));
        sprite_cadre.scale((float)(echelle *width)/ cadre_menu->getSize().x, (float)(echelle *height)/cadre_menu->getSize().y);

        texture_bouton.loadFromFile("ressources/menu_assets/case_menu_principal.png");
        texture_bouton_selec.loadFromFile("ressources/menu_assets/case_menu_principal_transparent.png");

        float centrage = ((((float)(echelle *height)/2.0)-(((float)noms_boutons.size()*(echelle *100.0))+(((float)noms_boutons.size()-1.0)*(echelle *20)))/2));
        for (size_t i = 0; i < noms_boutons.size(); i++)
        {
            Button *b = new Button(echelle, noms_boutons[i], sf::Vector2f(width-200, 100), 30, font, &texture_bouton, &texture_bouton_selec);
            b->setPosition(sf::Vector2f((echelle * position.x) +(echelle *100), (echelle * position.y) + centrage + (echelle *(i * 120))));
            boutons.push_back(b);
        }
    }

    ~MenuGui(){
        for(auto &b: boutons){
            delete b;
        }
        boutons.clear();
    }

    std::string getNomMenu(){
        return nom_menu;
    }

    void setIds(std::vector<int> ids){
        for(size_t i = 0; i < ids.size(); i++){
            boutons[i]->setId(ids[i]);
        }
    }

    void drawTo(sf::RenderWindow &fenetre)
    {
        fenetre.draw(sprite_cadre);
        for (auto &b : boutons)
        {
            b->drawTo(fenetre);
        }
    }

    std::vector<Button*>* getBoutonsMenu(){
        return &boutons;
    }

    float getWidth(){
        return width;
    }

    void update(sf::Vector2i position, int &ecran)
    {
        for (auto &b : boutons)
        {
            if (b->contains(position.x, position.y))
            {
                ecran = b->getId();
            }
        }
    }

    void mouseMove(sf::Vector2i position){
        for(auto &b: boutons){
            b->isMouseOver(position.x, position.y);
        }
    }
};

#endif