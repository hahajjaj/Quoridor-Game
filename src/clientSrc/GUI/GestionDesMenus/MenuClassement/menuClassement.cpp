#include "menuClassement.hpp"
#include "../../../Interface_GUI.hpp"



MenuClassement::MenuClassement(float echelle, int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, GUI *interface) : echelle(echelle), width(width), height(height), font_jeu(font_jeu), position(position), client(client), interface(interface)
{
    loadSprite();
}

MenuClassement::~MenuClassement()
{
    
}

void MenuClassement::loadSprite()
{
    bouton_petit.loadFromFile("ressources/menu_jeu/bouton_petit.png");
    bouton_petit_selec.loadFromFile("ressources/menu_jeu/bouton_petit_selec.png");
    cadre.loadFromFile("ressources/menu_jeu/cadre.png");
    next.loadFromFile("ressources/menu_jeu/next.png");
    next_selec.loadFromFile("ressources/menu_jeu/next_selec.png");
    onglet.loadFromFile("ressources/menu_jeu/onglet.png");
    onglet_selec.loadFromFile("ressources/menu_jeu/onglet_selec.png");
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

    rank.setString("Rank");
    rank.setOutlineThickness(echelle * 1);
    rank.setFont(*font_jeu);
    rank.setCharacterSize(echelle * 25);

    pseudo.setString("Pseudo");
    pseudo.setOutlineThickness(echelle * 1);
    pseudo.setFont(*font_jeu);
    pseudo.setCharacterSize(echelle * 25);

    score.setString("Score");
    score.setOutlineThickness(echelle * 1);
    score.setFont(*font_jeu);
    score.setCharacterSize(echelle * 25);

    ecart = ((spriteSousTableauXL.getGlobalBounds().width - (score.getGlobalBounds().width + pseudo.getGlobalBounds().width + rank.getGlobalBounds().width))/4.0);

    rank.setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + ecart, spriteTableau.getPosition().y + (echelle * 20)));
    pseudo.setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + (2 * ecart) + rank.getGlobalBounds().width, spriteTableau.getPosition().y + (echelle * 20)));
    score.setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + (3 * ecart)+ rank.getGlobalBounds().width + pseudo.getGlobalBounds().width, spriteTableau.getPosition().y + (echelle * 20)));



}

void MenuClassement::loadButtons()
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

    Button *bouton_onglet_1 = new Button(interface->getEchelle(), "Global", sf::Vector2f(64 * 3, 8 * 3), 20, font_jeu, &onglet, &onglet_selec);
    bouton_onglet_1->setPosition(sf::Vector2f(spriteTableau.getPosition().x, spriteTableau.getPosition().y - (interface->getEchelle() * (onglet.getSize().y * 3))));
    bouton_onglet_1->setNomBouton("onglet1");
    bouton_onglet_1->setTextColor(sf::Color::Green);
    bouton_onglet_1->setId(0);
    boutons_onglets.push_back(bouton_onglet_1);

    Button *bouton_onglet_2 = new Button(interface->getEchelle(), "Amis", sf::Vector2f(64 * 3, 8 * 3), 20, font_jeu, &onglet, &onglet_selec);
    bouton_onglet_2->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * (onglet.getSize().x * 3)), spriteTableau.getPosition().y - (interface->getEchelle() * (onglet.getSize().y * 3))));
    bouton_onglet_2->setNomBouton("onglet2");
    bouton_onglet_2->setId(1);
    boutons_onglets.push_back(bouton_onglet_2);

    Button *bouton_onglet_3 = new Button(interface->getEchelle(), "Hebdomadaire", sf::Vector2f(64 * 3, 8 * 3), 20, font_jeu, &onglet, &onglet_selec);
    bouton_onglet_3->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * (2 * (onglet.getSize().x * 3))), spriteTableau.getPosition().y - (interface->getEchelle() * (onglet.getSize().y * 3))));
    bouton_onglet_3->setNomBouton("onglet3");
    bouton_onglet_3->setId(2);
    boutons_onglets.push_back(bouton_onglet_3);
}

void MenuClassement::setPosition()
{
    position = sf::Vector2f((width / 2.0) - (spriteCadreMenu.getGlobalBounds().width / 2.0), (height / 2.0) - (spriteCadreMenu.getGlobalBounds().height / 2.0));

    spriteCadreMenu.setPosition(position);
    spriteTableau.setPosition(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteTableau.getGlobalBounds().width / 2.0), position.y + (spriteCadreMenu.getGlobalBounds().height / 2.0) - (spriteTableau.getGlobalBounds().height / 2.0));
    spriteSousTableauXL.setPosition(sf::Vector2f(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteSousTableauXL.getGlobalBounds().width / 2.0), spriteTableau.getPosition().y + (spriteTableau.getGlobalBounds().height / 2.0) - (spriteSousTableauXL.getGlobalBounds().height / 2.0)));

}

void MenuClassement::drawTo(sf::RenderWindow &fenetre)
{
    fenetre.draw(spriteCadreMenu);
    fenetre.draw(spriteTableau);
    fenetre.draw(spriteSousTableauXL);
    fenetre.draw(rank);
    fenetre.draw(pseudo);
    fenetre.draw(score);

    for (auto &b : bouton_generals)
    {
        b->drawTo(fenetre);
    }
    for (auto &b : boutons_onglets)
    {
        b->drawTo(fenetre);
    }
    for(int i = 0; static_cast<size_t>(i) < TextLigne_classement.size(); i++){
        if(i >= page_classement * 27 && i < (page_classement+1) * 27){
            fenetre.draw(TextLigne_classement[i]);
        }
    }
}

void MenuClassement::mouseMove(sf::Vector2i pos)
{
    for (auto &b : bouton_generals)
    {
        b->isMouseOver(pos.x, pos.y);
    }
    for (auto &b : boutons_onglets)
    {
        b->isMouseOver(pos.x, pos.y);
    }
    


}

void MenuClassement::mouseClick(sf::Vector2i pos, int &ecran_actuel)
{
    for(auto &b: bouton_generals){
        if(b->contains(pos.x, pos.y)){
            if (b->getNomBouton() == "nextPage")
            {
                if(page_onglet == 0 && static_cast<int>(lignes.size()) > (page_classement+1)*9){
                    page_classement++;
                }else if (page_onglet == 1 && static_cast<int>(lignes.size()) > (page_classement+1)*9){
                    page_classement++;
                }
                else if(page_onglet == 2 && static_cast<int>(lignes.size()) > (page_classement+1)*9){
                    page_classement++;
                }
            }
            if (b->getNomBouton() == "previousPage")
            {
                if(page_onglet == 0 && page_classement > 0){
                    page_classement--;
                }else if (page_onglet == 1 && page_classement > 0){
                    page_classement--;
                }
                else if(page_onglet == 2 && page_classement > 0){
                    page_classement--;
                }
            }
            if(b->getNomBouton() == "retour"){
                page_onglet = 0;
                TextLigne_classement.clear();
                ligne_classement.clear();
                ecran_actuel = 1;
            }
        }
    }
    for (auto &b : boutons_onglets)
    {
        if (b->contains(pos.x, pos.y))
        {
            if (b->getNomBouton() == "onglet1")
            {
                page_onglet = 0;
                page_classement = 0;
                parseRowClassement();
            }
            if (b->getNomBouton() == "onglet2")
            {
                page_classement = 0;
                page_onglet = 1;
                parseRowClassement();
            }
            if (b->getNomBouton() == "onglet3")
            {
                page_classement = 0;
                page_onglet = 2;
                parseRowClassement();
            }
        }
    }
    for (auto &b : boutons_onglets)
    {
        if (b->getId() == page_onglet)
        {
            b->setTextColor(sf::Color::Green);
        }
        else
        {
            b->setTextColor(sf::Color::White);
        }
    }
}

void MenuClassement::parseRowClassement()
{
    client->sendMessage(genre_classement[page_onglet]);
    std::string res = client->getMessage();
    parseMessageClient(res, &ligne_classement);
    
    lignes.clear();
    TextLigne_classement.clear();
    float ecart = echelle * 50;
    for(int i = 0; static_cast<size_t>(i) < ligne_classement.size(); i ++){
        parseRowPosition(ligne_classement[i], i);
        sf::Text l(to_string(i+1), *font_jeu, (echelle * 25));
        l.setOutlineThickness(echelle * 2);
        sf::Text l1(lignes[i][0], *font_jeu, (echelle * 25));
        l1.setOutlineThickness(echelle * 2);
        sf::Text l2(lignes[i][1], *font_jeu, (echelle * 25));
        l2.setOutlineThickness(echelle * 2);
        l.setPosition(sf::Vector2f(rank.getPosition().x, spriteSousTableauXL.getPosition().y + (echelle * 20) + ((i % 9) * ecart)));
        l1.setPosition(sf::Vector2f(pseudo.getPosition().x, spriteSousTableauXL.getPosition().y + (echelle * 20) + ((i % 9) * ecart)));
        l2.setPosition(sf::Vector2f(score.getPosition().x, spriteSousTableauXL.getPosition().y + (echelle * 20) + ((i % 9) * ecart)));

        TextLigne_classement.push_back(l);
        TextLigne_classement.push_back(l1);
        TextLigne_classement.push_back(l2);
    }
}

void MenuClassement::parseMessageClient(std::string s, std::vector<std::string> *liste)
{
    liste->clear();
    s.erase(0, 1);
    std::string delimiter = "\n";
    auto start = 0U;
    auto end = s.find(delimiter);
    while (end != std::string::npos)
    {
        if (s.substr(start, end - start) != "")
        {
            liste->push_back(s.substr(start, end - start));
        }
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
}

void MenuClassement::parseRowPosition(std::string s, int indice)
{
    // lignes.clear();
    // s.erase(0, 1);
    std::string delimiter = " ";
    auto start = 0U;
    auto end = s.find(delimiter);
    lignes.push_back({});
    while (end != std::string::npos)
    {
        if (s.substr(start, end - start) != "")
        {
            // std::cout << s.substr(start, end - start) << std::endl;
            lignes[indice].push_back(s.substr(start, end - start));
        }
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
}

void MenuClassement::update()
{
    parseRowClassement();
}
