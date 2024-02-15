#include "rowFriend.hpp"

rowFriend::rowFriend(float echelle, std::string nom_ami, sf::Font *font_jeu, sf::Vector2f position) : echelle(echelle), font_jeu(font_jeu), position(position)
{
    this->nom_ami.setString(nom_ami);
    this->nom_ami.setPosition(position.x, position.y);
    this->nom_ami.setFont(*font_jeu);
    this->nom_ami.setOutlineThickness(echelle * 1);
    this->nom_ami.setCharacterSize(echelle * 25);

    // this->nom_bouton_chat.setString("Chatter");
    loadRess();
}

void rowFriend::loadRess(){
    bouton.loadFromFile("ressources/menu_ami_assets/bouton_x.png");
    bouton_selec.loadFromFile("ressources/menu_ami_assets/bouton_x_selec.png");
    bouton_chat.loadFromFile("ressources/menu_ami_assets/chat.png");

    Button *bouton_delete = new Button(echelle, "", sf::Vector2f(50, 50), 25, font_jeu, &bouton, &bouton_selec);
    bouton_delete->setNomBouton("2");
    bouton_delete->setPosition(sf::Vector2f(position.x + (echelle * 20), position.y));
    bouton_delete->setId(1);
    bouton_delete->setPosition(sf::Vector2f(position.x + (echelle * 480), position.y));
    boutons.push_back(bouton_delete);

    Button *boutonChat = new Button(echelle, "Chatter", sf::Vector2f(100, 40), 25, font_jeu, &bouton_chat, &bouton_chat);
    boutonChat->setNomBouton("1");
    boutonChat->setPosition(sf::Vector2f(position.x + (echelle * 20), position.y));
    boutonChat->setTextColor(sf::Color::Black);
    boutonChat->setId(1);
    boutonChat->setPosition(sf::Vector2f(position.x + (echelle * 350), position.y));

    boutons.push_back(boutonChat);
}

void rowFriend::setPosition(sf::Vector2f pos){
    
}

void rowFriend::drawTo(sf::RenderWindow &fenetre){
    fenetre.draw(nom_ami);
    for(auto &b: boutons){
        b->drawTo(fenetre);
    }
}

bool rowFriend::mouseMove(sf::Vector2i pos){
    for(auto &b: boutons){
        if(b->isMouseOver(pos.x, pos.y)){
            return true;
        }
    }
    return false;
}

bool rowFriend::mouseClick(sf::Vector2i pos, int &ecran_actuel){
    for(auto &b: boutons){
        if(b->isMouseOver(pos.x, pos.y)){
            return true;
        }
    }
    return false;
}