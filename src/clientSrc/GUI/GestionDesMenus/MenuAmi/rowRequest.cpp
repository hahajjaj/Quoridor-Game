#include "rowRequest.hpp"

rowRequest::rowRequest(float echelle, std::string nom_ami, sf::Font *font_jeu, sf::Vector2f position,int sizeChar, bool two_buttons, sf::Vector2f size) : echelle(echelle), font_jeu(font_jeu), position(position), sizeChar(sizeChar),  two_button(two_buttons), bouton_size(size)
{
    this->nom_ami.setString(nom_ami);
    this->nom_ami.setPosition(position.x, position.y);
    this->nom_ami.setFont(*font_jeu);
    this->nom_ami.setCharacterSize(echelle * sizeChar);
    this->nom_ami.setOutlineThickness(echelle * 1);

    // this->nom_bouton_chat.setString("Chatter");
    loadRess();
}

void rowRequest::loadRess(){
    bouton.loadFromFile("ressources/menu_ami_assets/bouton_x.png");
    bouton_selec.loadFromFile("ressources/menu_ami_assets/bouton_x_selec.png");

    Button *bouton_delete = new Button(echelle, "", sf::Vector2f(bouton_size.x, bouton_size.y), sizeChar, font_jeu, &bouton, &bouton_selec);
    bouton_delete->setPosition(sf::Vector2f(position.x + (echelle * 290), position.y));
    bouton_delete->setNomBouton("2");
    boutons.push_back(bouton_delete);

    if(two_button){
        bouton_v.loadFromFile("ressources/menu_ami_assets/bouton_v.png");
        bouton_v_selec.loadFromFile("ressources/menu_ami_assets/bouton_v_selec.png");

        Button *bouton_accept = new Button(echelle, "", sf::Vector2f(bouton_size.x, bouton_size.y), sizeChar, font_jeu, &bouton_v, &bouton_v_selec);
        bouton_accept->setPosition(sf::Vector2f(position.x + (echelle * 230), position.y));
        bouton_accept->setNomBouton("1");
        boutons.push_back(bouton_accept);
    }
}

void rowRequest::setPosition(sf::Vector2f pos){
    for(auto &b: boutons){
        b->setPosition(sf::Vector2f(pos.x, pos.y));
    }
}


void rowRequest::setTexture(sf::Texture *text, sf::Texture *text2, sf::Vector2f size, std::string new_text){
    for(auto &b: boutons){
        b->setTexture(text, text2, size);
        b->setText(new_text);
    }
}

void rowRequest::drawTo(sf::RenderWindow &fenetre){
    fenetre.draw(nom_ami);
    for(auto &b: boutons){
        b->drawTo(fenetre);
    }
}

bool rowRequest::mouseMove(sf::Vector2i pos){
    for(auto &b: boutons){
        if(b->isMouseOver(pos.x, pos.y)){
            return true;
        }
    }
    return false;
}

bool rowRequest::mouseClick(sf::Vector2i pos, int &ecran_actuel){
    for(auto &b: boutons){
        if(b->isMouseOver(pos.x, pos.y)){
            return true;
        }
    }
    return false;
}