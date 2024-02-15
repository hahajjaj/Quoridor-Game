#include "menuReglement.hpp"
#include "../../../Interface_GUI.hpp"

MenuReglement::MenuReglement(float echelle, int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, GUI *interface) : echelle(echelle), width(width), height(height), font_jeu(font_jeu), position(position), client(client), interface(interface)
{
    regles.setString("BUT DU JEU\n\
Atteindre le premier la ligne opposee a sa ligne de depart\n\
REGLE POUR 2 JOUEURS\n\
En debut de partie, les barrieres sont remises dans leur zone de \nstockage (10 par joueur).\n\
Chaque joueur pose son pion au centre de sa ligne de depart.\n\
Un tirage au sort determine qui commence.\n\
Deroulement d\'une partie\n\
A tour de role, chaque joueur choisit de deplacer son pion ou \nde poser une de ses barrieres.\n\
Lorsqu\'il n\'a plus de barrieres, un joueur est oblige de \ndeplacer son pion. Deplacement des pions:\n\
les pions se deplacent d\'une case, horizontalement ou \nverticalement, en avant ou en arriere :\n\
les barrieres doivent etre contournees.\n\
Pose des barrieres:\n\
une barriere doit etre posee exactement entre 2 blocs de 2 \ncases.\n\
La pose des barrieres a pour but de se creer son propre \nchemin ou de ralentir l\'adversaire, mais il est interdit de lui fermer \ntotalement l\'acces a sa ligne de but:\n\
il faut toujours lui laisser une solution.\n\
Face a face:\n\
Quand les 2 pions se retrouvent en vis-a-vis sur 2 cases \nvoisines non separees par une barriere,\n\
le joueur dont c\'est le tour peut sauter son adversaire et \n");
    regles2.setString("se placer derriere lui.\n\
Si une barriere est situee derriere le pion saute, le joueur \npeut choisir de bifurquer a droite ou a gauche du pion saute.\n\
FIN DE LA PARTIE\n\
Le premier joueur qui atteint une des 9 cases de la ligne \nopposee a sa ligne de depart gagne la partie.\n\
REGLE POUR 4 JOUEURS\n\
En debut de partie, les 4 pions sont disposes au centre de \nchacun des 4 cotes du plateau et chaque joueur dispose de \n5 barrieres.\n\
Les regles sont strictement identiques, mais on ne peut \nsauter plus d\'un pion a la fois");
    regles.setFont(*font_jeu);
    regles.setCharacterSize(echelle * 18);
    regles.setOutlineThickness(echelle * 2);

    regles2.setFont(*font_jeu);
    regles2.setCharacterSize(echelle * 18);
    regles2.setOutlineThickness(echelle * 2);

    loadSprite();
}

MenuReglement::~MenuReglement()
{
    
}

void MenuReglement::loadSprite()
{
    bouton_petit.loadFromFile("ressources/menu_jeu/bouton_petit.png");
    bouton_petit_selec.loadFromFile("ressources/menu_jeu/bouton_petit_selec.png");
    cadre.loadFromFile("ressources/menu_jeu/cadre.png");
    next.loadFromFile("ressources/menu_jeu/next.png");
    next_selec.loadFromFile("ressources/menu_jeu/next_selec.png");
    previous.loadFromFile("ressources/menu_jeu/previous.png");
    previous_selec.loadFromFile("ressources/menu_jeu/previous_selec.png");
    tableau.loadFromFile("ressources/menu_jeu/tableau.png");
    sousTableau_ecranSecondaires.loadFromFile("ressources/menu_jeu/sousTableau_ecranSecondaires.png");

    spriteCadreMenu.setTexture(cadre);
    spriteCadreMenu.scale((float)(interface->getEchelle() * 672) / cadre.getSize().x, (float)(interface->getEchelle() * 696) / cadre.getSize().y);

    spriteTableau.setTexture(tableau);
    spriteTableau.scale((float)(interface->getEchelle() * 576) / tableau.getSize().x, (float)(interface->getEchelle() * 576) / tableau.getSize().y);

    spriteSousTableauXL.setTexture(sousTableau_ecranSecondaires);
    spriteSousTableauXL.scale((interface->getEchelle() * sousTableau_ecranSecondaires.getSize().x * 3) / sousTableau_ecranSecondaires.getSize().x, (interface->getEchelle() * sousTableau_ecranSecondaires.getSize().y * 3) / sousTableau_ecranSecondaires.getSize().y);

    setPosition();

    loadButtons();
}

void MenuReglement::loadButtons()
{
    Button *bouton_page_next = new Button(interface->getEchelle(), "", sf::Vector2f(40, 40), 30, font_jeu, &next, &next_selec);
    bouton_page_next->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * 225) + (interface->getEchelle() * 40) + (interface->getEchelle() * 46), spriteTableau.getPosition().y + spriteTableau.getGlobalBounds().height - (interface->getEchelle() * 50)));
    bouton_page_next->setNomBouton("nextPage");
    bouton_generals.push_back(bouton_page_next);

    Button *bouton_page_previous = new Button(interface->getEchelle(), "", sf::Vector2f(40, 40), 30, font_jeu, &previous, &previous_selec);
    bouton_page_previous->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * 225), spriteTableau.getPosition().y + spriteTableau.getGlobalBounds().height - (interface->getEchelle() * 50)));
    bouton_page_previous->setNomBouton("previousPage");
    bouton_generals.push_back(bouton_page_previous);

    Button *bouton_retour = new Button(interface->getEchelle(), "Retour", sf::Vector2f(150, 50), 25, font_jeu, &bouton_petit, &bouton_petit_selec);
    bouton_retour->setPosition(sf::Vector2f(spriteCadreMenu.getPosition().x + (interface->getEchelle() * 20), spriteCadreMenu.getPosition().y + spriteCadreMenu.getGlobalBounds().height));
    bouton_retour->setNomBouton("retour");
    bouton_generals.push_back(bouton_retour);
}

void MenuReglement::setPosition()
{
    position = sf::Vector2f((width / 2.0) - (spriteCadreMenu.getGlobalBounds().width / 2.0), (height / 2.0) - (spriteCadreMenu.getGlobalBounds().height / 2.0));
    
    spriteCadreMenu.setPosition(position);
    spriteTableau.setPosition(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteTableau.getGlobalBounds().width / 2.0), position.y + (spriteCadreMenu.getGlobalBounds().height / 2.0) - (spriteTableau.getGlobalBounds().height / 2.0));
    spriteSousTableauXL.setPosition(sf::Vector2f(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteSousTableauXL.getGlobalBounds().width / 2.0), spriteTableau.getPosition().y + (spriteTableau.getGlobalBounds().height / 2.0) - (spriteSousTableauXL.getGlobalBounds().height / 2.0)));
    regles.setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + 20, spriteSousTableauXL.getPosition().y + 20));
    regles2.setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + 20, spriteSousTableauXL.getPosition().y + 20));

}

void MenuReglement::drawTo(sf::RenderWindow &fenetre)
{
    fenetre.draw(spriteCadreMenu);
    fenetre.draw(spriteTableau);
    fenetre.draw(spriteSousTableauXL);
    if(page == 0){
        fenetre.draw(regles);
    }else if (page == 1){
        fenetre.draw(regles2);
    }
    for(auto &b: bouton_generals){
        b->drawTo(fenetre);
    }
}

void MenuReglement::mouseMove(sf::Vector2i pos)
{
    for (auto &b : bouton_generals)
    {
        b->isMouseOver(pos.x, pos.y);
    }
}

void MenuReglement::mouseClick(sf::Vector2i pos, int &ecran_actuel)
{
    for(auto &b: bouton_generals){
        if(b->contains(pos.x, pos.y)){
            if (b->getNomBouton() == "nextPage" && page == 0)
            {
                page++;
            }
            if (b->getNomBouton() == "previousPage" && page >0)
            {
                page--;
            }
            if(b->getNomBouton() == "retour"){
                
                ecran_actuel = 1;
            }
        }
    }
}

void MenuReglement::update()
{
    
}
