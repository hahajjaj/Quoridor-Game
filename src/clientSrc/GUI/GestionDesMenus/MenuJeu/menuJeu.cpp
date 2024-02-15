
#include "menuJeu.hpp"
#include "../../../Interface_GUI.hpp"

MenuJeu::MenuJeu(int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, MenuGui &menu_principal, GUI *interface) : width(width), height(height), font_jeu(font_jeu), position(position), client(client), interface(interface)
{
    this->menu_principal = &menu_principal;
    loadSprite();
}

MenuJeu::~MenuJeu()
{
}

void MenuJeu::loadSprite()
{
    choix_mode.loadFromFile("ressources/menu_jeu/bouton_grand.png");
    bouton_petit.loadFromFile("ressources/menu_jeu/bouton_petit.png");
    bouton_petit_selec.loadFromFile("ressources/menu_jeu/bouton_petit_selec.png");
    cadre.loadFromFile("ressources/menu_jeu/cadre.png");
    next.loadFromFile("ressources/menu_jeu/next.png");
    next_selec.loadFromFile("ressources/menu_jeu/next_selec.png");
    onglet.loadFromFile("ressources/menu_jeu/onglet.png");
    onglet_selec.loadFromFile("ressources/menu_jeu/onglet_selec.png");
    previous.loadFromFile("ressources/menu_jeu/previous.png");
    previous_selec.loadFromFile("ressources/menu_jeu/previous_selec.png");
    sous_tableau_grand.loadFromFile("ressources/menu_jeu/sous_tableau_grand.png");
    sous_tableau_petit.loadFromFile("ressources/menu_jeu/sous_tableau_petit.png");
    tableau.loadFromFile("ressources/menu_jeu/tableau.png");
    sousTableau_ecranSecondaires.loadFromFile("ressources/menu_jeu/sousTableau_ecranSecondaires.png");

    spriteCadreMenu.setTexture(cadre);
    spriteCadreMenu.scale((float)(interface->getEchelle() * 672) / cadre.getSize().x, (float)(interface->getEchelle() * 696) / cadre.getSize().y);

    spriteTableau.setTexture(tableau);
    spriteTableau.scale((float)(interface->getEchelle() * 576) / tableau.getSize().x, (float)(interface->getEchelle() * 576) / tableau.getSize().y);

    spriteSousTableau_petit.setTexture(sous_tableau_petit);
    spriteSousTableau_petit.scale((float)(interface->getEchelle() * 480) / sous_tableau_petit.getSize().x, (float)(interface->getEchelle() * 168) / sous_tableau_petit.getSize().y);

    spriteSousTableau_grand.setTexture(sous_tableau_grand);
    spriteSousTableau_grand.scale((float)(interface->getEchelle() * 480) / sous_tableau_grand.getSize().x, (float)(interface->getEchelle() * 288) / sous_tableau_grand.getSize().y);

    spriteChoixMode.setTexture(choix_mode);
    spriteChoixMode.scale((float)(interface->getEchelle() * 192) / choix_mode.getSize().x, (float)(interface->getEchelle() * 48) / choix_mode.getSize().y);

    spriteChoixJoueur.setTexture(choix_mode);
    spriteChoixJoueur.scale((float)(interface->getEchelle() * 192) / choix_mode.getSize().x, (float)(interface->getEchelle() * 48) / choix_mode.getSize().y);

    spriteSousTableauXL.setTexture(sousTableau_ecranSecondaires);
    spriteSousTableauXL.scale((interface->getEchelle() * sousTableau_ecranSecondaires.getSize().x * 3) / sousTableau_ecranSecondaires.getSize().x, (interface->getEchelle() * sousTableau_ecranSecondaires.getSize().y * 3) / sousTableau_ecranSecondaires.getSize().y);

    setPosition();

    mode.setString("Mode de jeu");
    mode.setOutlineThickness((interface->getEchelle() * 1));
    mode.setFont(*font_jeu);
    mode.setCharacterSize((interface->getEchelle() * 20));

    nbrJoueur.setString("Nombre joueurs");
    nbrJoueur.setOutlineThickness((interface->getEchelle() * 1));
    nbrJoueur.setFont(*font_jeu);
    nbrJoueur.setCharacterSize((interface->getEchelle() * 20));

    nom_mode.setString(mode_de_jeu[indice_mode_de_jeu]);
    nom_mode.setOutlineThickness(interface->getEchelle() * 1);
    nom_mode.setFont(*font_jeu);
    nom_mode.setCharacterSize((interface->getEchelle() * 25));

    nom_nbrJoueur.setString(nombre_joueurs[indice_nombre_joueur]);
    nom_nbrJoueur.setOutlineThickness(interface->getEchelle() * 1);
    nom_nbrJoueur.setFont(*font_jeu);
    nom_nbrJoueur.setCharacterSize((interface->getEchelle() * 25));

    annonce.setOutlineThickness((interface->getEchelle() * 1));
    annonce.setFont(*font_jeu);
    annonce.setCharacterSize((interface->getEchelle() * 30));

    float ecart_init_jeu = (spriteSousTableau_petit.getGlobalBounds().height - (mode.getGlobalBounds().height)) / 3.0;
    mode.setPosition(sf::Vector2f(spriteSousTableau_petit.getPosition().x + (interface->getEchelle() * 20), spriteSousTableau_petit.getPosition().y - (interface->getEchelle() * 10) + ecart_init_jeu - mode.getGlobalBounds().height));
    nbrJoueur.setPosition(sf::Vector2f(spriteSousTableau_petit.getPosition().x + (interface->getEchelle() * 20), spriteSousTableau_petit.getPosition().y + (interface->getEchelle() * 10) + (2 * ecart_init_jeu - 2 * (mode.getGlobalBounds().height)) + mode.getGlobalBounds().height));

    nom_mode.setPosition(sf::Vector2f(spriteChoixMode.getPosition().x + (spriteChoixMode.getGlobalBounds().width / 2.0) - (nom_mode.getGlobalBounds().width / 2.0), spriteChoixMode.getPosition().y + (spriteChoixMode.getGlobalBounds().height / 2.0) - (interface->getEchelle() * 22)));
    nom_nbrJoueur.setPosition(sf::Vector2f(spriteChoixJoueur.getPosition().x + (spriteChoixJoueur.getGlobalBounds().width / 2.0) - (nom_nbrJoueur.getGlobalBounds().width / 2.0), spriteChoixJoueur.getPosition().y + (spriteChoixJoueur.getGlobalBounds().height / 2.0) - (interface->getEchelle() * 22)));
    loadButtons();
}

void MenuJeu::loadButtons()
{
    Button *bouton_mode_next = new Button(interface->getEchelle(), "", sf::Vector2f(45, 45), 30, font_jeu, &next, &next_selec);
    bouton_mode_next->setPosition(sf::Vector2f(spriteChoixMode.getPosition().x + spriteChoixMode.getGlobalBounds().width + (interface->getEchelle() * 5), spriteChoixMode.getPosition().y));
    bouton_mode_next->setNomBouton("mode_next");
    boutons1.push_back(bouton_mode_next);

    Button *bouton_mode_previous = new Button(interface->getEchelle(), "", sf::Vector2f(45, 45), 30, font_jeu, &previous, &previous_selec);
    bouton_mode_previous->setPosition(sf::Vector2f(spriteChoixMode.getPosition().x - (interface->getEchelle() * 55), spriteChoixMode.getPosition().y));
    bouton_mode_previous->setNomBouton("mode_previous");
    boutons1.push_back(bouton_mode_previous);

    Button *bouton_joueur_next = new Button(interface->getEchelle(), "", sf::Vector2f(45, 45), 30, font_jeu, &next, &next_selec);
    bouton_joueur_next->setPosition(sf::Vector2f(spriteChoixJoueur.getPosition().x + spriteChoixJoueur.getGlobalBounds().width + (interface->getEchelle() * 5), spriteChoixJoueur.getPosition().y));
    bouton_joueur_next->setNomBouton("nbrJoueur_next");
    boutons1.push_back(bouton_joueur_next);

    Button *bouton_joueur_previous = new Button(interface->getEchelle(), "", sf::Vector2f(45, 45), 30, font_jeu, &previous, &previous_selec);
    bouton_joueur_previous->setPosition(sf::Vector2f(spriteChoixJoueur.getPosition().x - (interface->getEchelle() * 55), spriteChoixJoueur.getPosition().y));
    bouton_joueur_previous->setNomBouton("nbrJoueur_previous");
    boutons1.push_back(bouton_joueur_previous);

    Button *bouton_page_next = new Button(interface->getEchelle(), "", sf::Vector2f(40, 40), 30, font_jeu, &next, &next_selec);
    bouton_page_next->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * 225) + (interface->getEchelle() * 40) + (interface->getEchelle() * 46), spriteTableau.getPosition().y + spriteTableau.getGlobalBounds().height - (interface->getEchelle() * 50)));
    bouton_page_next->setNomBouton("nextPage");
    bouton_generals.push_back(bouton_page_next);

    Button *bouton_page_previous = new Button(interface->getEchelle(), "", sf::Vector2f(40, 40), 30, font_jeu, &previous, &previous_selec);
    bouton_page_previous->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * 225), spriteTableau.getPosition().y + spriteTableau.getGlobalBounds().height - (interface->getEchelle() * 50)));
    bouton_page_previous->setNomBouton("previousPage");
    bouton_generals.push_back(bouton_page_previous);

    Button *bouton_start = new Button(interface->getEchelle(), "Lancer partie", sf::Vector2f(220, 50), 30, font_jeu, &bouton_petit, &bouton_petit_selec);
    bouton_start->setPosition(sf::Vector2f(spriteCadreMenu.getPosition().x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (interface->getEchelle() * 110), spriteCadreMenu.getPosition().y + spriteCadreMenu.getGlobalBounds().height));
    bouton_start->setNomBouton("start");
    boutons1.push_back(bouton_start);

    Button *bouton_retour = new Button(interface->getEchelle(), "Retour", sf::Vector2f(150, 50), 25, font_jeu, &bouton_petit, &bouton_petit_selec);
    bouton_retour->setPosition(sf::Vector2f(spriteCadreMenu.getPosition().x + (interface->getEchelle() * 20), spriteCadreMenu.getPosition().y + spriteCadreMenu.getGlobalBounds().height));
    bouton_retour->setNomBouton("retour");
    bouton_generals.push_back(bouton_retour);

    Button *bouton_onglet_1 = new Button(interface->getEchelle(), "Creer partie", sf::Vector2f(64 * 3, 8 * 3), 20, font_jeu, &onglet, &onglet_selec);
    bouton_onglet_1->setPosition(sf::Vector2f(spriteTableau.getPosition().x, spriteTableau.getPosition().y - (interface->getEchelle() * (onglet.getSize().y * 3))));
    bouton_onglet_1->setNomBouton("onglet1");
    bouton_onglet_1->setTextColor(sf::Color::Green);
    bouton_onglet_1->setId(0);
    boutons_onglets.push_back(bouton_onglet_1);

    Button *bouton_onglet_2 = new Button(interface->getEchelle(), "Rejoindre partie", sf::Vector2f(64 * 3, 8 * 3), 20, font_jeu, &onglet, &onglet_selec);
    bouton_onglet_2->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * (onglet.getSize().x * 3)), spriteTableau.getPosition().y - (interface->getEchelle() * (onglet.getSize().y * 3))));
    bouton_onglet_2->setNomBouton("onglet2");
    bouton_onglet_2->setId(1);
    boutons_onglets.push_back(bouton_onglet_2);

    Button *bouton_onglet_3 = new Button(interface->getEchelle(), "Sauvegardes", sf::Vector2f(64 * 3, 8 * 3), 20, font_jeu, &onglet, &onglet_selec);
    bouton_onglet_3->setPosition(sf::Vector2f(spriteTableau.getPosition().x + (interface->getEchelle() * (2 * (onglet.getSize().x * 3))), spriteTableau.getPosition().y - (interface->getEchelle() * (onglet.getSize().y * 3))));
    bouton_onglet_3->setNomBouton("onglet3");
    bouton_onglet_3->setId(2);
    boutons_onglets.push_back(bouton_onglet_3);

    float ecart = (interface->getEchelle() * 50);
    liste_demandes_row.clear();
    for (size_t i = 0; i < friendlist.size(); i++)
    {
        rowRequest *r = new rowRequest(interface->getEchelle(), friendlist[i], font_jeu, sf::Vector2f(spriteSousTableau_grand.getPosition().x + (interface->getEchelle() * 20), spriteSousTableau_grand.getPosition().y + (interface->getEchelle() * 20) + ((i % 13) * ecart)), 20, false, sf::Vector2f(100, 40));
        r->setTexture(&bouton_petit, &bouton_petit_selec, sf::Vector2f(100, 40), "Inviter");
        r->setPosition(sf::Vector2f(spriteSousTableau_grand.getPosition().x + spriteSousTableau_grand.getGlobalBounds().width - (interface->getEchelle() * 20) - (interface->getEchelle() * 100), spriteSousTableau_grand.getPosition().y + (interface->getEchelle() * 20) + ((i % 13) * ecart)));
        liste_demandes_row.push_back(r);
    }

    liste_demandes_partie_row.clear();
    for (size_t i = 0; i < invitelist.size(); i++)
    {
        rowRequest *r = new rowRequest(interface->getEchelle(), invitelist[i], font_jeu, sf::Vector2f(spriteSousTableauXL.getPosition().x + 20, spriteSousTableauXL.getPosition().y + 20 + ((i % 13) * ecart)), 20, false, sf::Vector2f(100, 40));
        r->setTexture(&bouton_petit, &bouton_petit_selec, sf::Vector2f(100, 40), "Rejoindre");
        r->setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + spriteSousTableauXL.getGlobalBounds().width - 20 - 100, spriteSousTableauXL.getPosition().y + 20 + ((i % 13) * ecart)));
        liste_demandes_partie_row.push_back(r);
    }

    liste_demandes_sauvegarde_row.clear();
    for (size_t i = 0; i < sauvegardelist.size(); i++)
    {
        rowRequest *r = new rowRequest(interface->getEchelle(), sauvegardelist[i], font_jeu, sf::Vector2f(spriteSousTableauXL.getPosition().x + 20, spriteSousTableauXL.getPosition().y + 20 + ((i % 13) * ecart)), 20, false, sf::Vector2f(100, 40));
        r->setTexture(&bouton_petit, &bouton_petit_selec, sf::Vector2f(100, 40), "Rejoindre");
        r->setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + spriteSousTableauXL.getGlobalBounds().width - 20 - 100, spriteSousTableauXL.getPosition().y + 20 + ((i % 13) * ecart)));
        liste_demandes_sauvegarde_row.push_back(r);
    }
}

void MenuJeu::setPosition()
{
    position = sf::Vector2f((width / 2.0) - (spriteCadreMenu.getGlobalBounds().width / 2.0), (height / 2.0) - (spriteCadreMenu.getGlobalBounds().height / 2.0));
    spriteCadreMenu.setPosition(position);
    spriteTableau.setPosition(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteTableau.getGlobalBounds().width / 2.0), position.y + (spriteCadreMenu.getGlobalBounds().height / 2.0) - (spriteTableau.getGlobalBounds().height / 2.0));
    float ecart_sous_tableau = (spriteTableau.getGlobalBounds().height - (spriteSousTableau_grand.getGlobalBounds().height + spriteSousTableau_petit.getGlobalBounds().height)) / 3.0;
    spriteSousTableau_petit.setPosition(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteSousTableau_petit.getGlobalBounds().width / 2.0), spriteTableau.getPosition().y + ecart_sous_tableau);
    spriteSousTableau_grand.setPosition(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteSousTableau_grand.getGlobalBounds().width / 2.0), spriteTableau.getPosition().y + (2 * ecart_sous_tableau) + spriteSousTableau_petit.getGlobalBounds().height);
    float ecart_bouton_choix = (spriteSousTableau_petit.getGlobalBounds().height - (2 * (spriteChoixMode.getGlobalBounds().height))) / 3.0;
    spriteChoixMode.setPosition(spriteSousTableau_petit.getPosition().x + spriteSousTableau_petit.getGlobalBounds().width - (interface->getEchelle() * 70) - spriteChoixMode.getGlobalBounds().width, spriteSousTableau_petit.getPosition().y + ecart_bouton_choix);
    spriteChoixJoueur.setPosition(spriteSousTableau_petit.getPosition().x + spriteSousTableau_petit.getGlobalBounds().width - (interface->getEchelle() * 70) - spriteChoixJoueur.getGlobalBounds().width, spriteSousTableau_petit.getPosition().y + (2 * ecart_bouton_choix) + spriteChoixMode.getGlobalBounds().height);
    spriteSousTableauXL.setPosition(sf::Vector2f(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteSousTableauXL.getGlobalBounds().width / 2.0), spriteTableau.getPosition().y + (spriteTableau.getGlobalBounds().height / 2.0) - (spriteSousTableauXL.getGlobalBounds().height / 2.0)));
}

void MenuJeu::parseConnectedFriends()
{
    client->sendMessage("fgc");
    std::string res = client->getMessage();
    parseMessageClient(res, &friendlist);

    float ecart = interface->getEchelle() * 50;
    liste_demandes_row.clear();
    for (size_t i = 0; i < friendlist.size(); i++)
    {
        rowRequest *r = new rowRequest(interface->getEchelle(), friendlist[i], font_jeu, sf::Vector2f(spriteSousTableau_grand.getPosition().x + 20, spriteSousTableau_grand.getPosition().y + 20 + ((i % 6) * ecart)), 20, false, sf::Vector2f(100, 40));
        r->setTexture(&bouton_petit, &bouton_petit_selec, sf::Vector2f(100, 40), "Inviter");
        r->setPosition(sf::Vector2f(spriteSousTableau_grand.getPosition().x + spriteSousTableau_grand.getGlobalBounds().width - (interface->getEchelle() * 20) - (interface->getEchelle() * 100), spriteSousTableau_grand.getPosition().y + (interface->getEchelle() * 20) + ((i % 6) * ecart)));
        if (std::find(joueurs_invite.begin(), joueurs_invite.end(), r->getNomAmi()) != joueurs_invite.end())
        {
            for (auto &b : r->getBoutons())
            {
                if (b->getNomCase() == "Inviter")
                {
                    b->setTextColor(sf::Color::Green);
                }
            }
        }
        liste_demandes_row.push_back(r);
    }
}

void MenuJeu::parseInvitation()
{
    client->sendMessage("ggi");
    std::string res = client->getMessage();
    parseMessageClient(res, &invitelist);

    float ecart = interface->getEchelle() * 50;
    liste_demandes_partie_row.clear();
    for (size_t i = 0; i < invitelist.size(); i++)
    {
        rowRequest *r = new rowRequest(interface->getEchelle(), invitelist[i], font_jeu, sf::Vector2f(spriteSousTableauXL.getPosition().x + 20, spriteSousTableauXL.getPosition().y + 20 + ((i % 9) * ecart)), 20, false, sf::Vector2f(100, 40));
        r->setTexture(&bouton_petit, &bouton_petit_selec, sf::Vector2f(100, 40), "Rejoindre");
        r->setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + spriteSousTableauXL.getGlobalBounds().width - (interface->getEchelle() * 20) - (interface->getEchelle() * 100), spriteSousTableauXL.getPosition().y + (interface->getEchelle() * 20) + ((i % 9) * ecart)));
        liste_demandes_partie_row.push_back(r);
    }
}

void MenuJeu::parseSaveInvit()
{
    client->sendMessage("ggs");
    std::string res = client->getMessage();
    parseMessageClient(res, &sauvegardelist);
    if (sauvegardelist.empty() && res[0] != '0')
    {
        res.erase(0, 1);
        sauvegardelist.push_back(res);
    }

    float ecart = interface->getEchelle() * 50;
    liste_demandes_sauvegarde_row.clear();
    for (size_t i = 0; i < sauvegardelist.size(); i++)
    {
        rowRequest *r = new rowRequest(interface->getEchelle(), sauvegardelist[i], font_jeu, sf::Vector2f(spriteSousTableauXL.getPosition().x + 20, spriteSousTableauXL.getPosition().y + 20 + ((i % 9) * ecart)), 20, false, sf::Vector2f(100, 40));
        r->setTexture(&bouton_petit, &bouton_petit_selec, sf::Vector2f(100, 40), "Rejoindre");
        r->setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + spriteSousTableauXL.getGlobalBounds().width - (interface->getEchelle() * 20) - (interface->getEchelle() * 100), spriteSousTableauXL.getPosition().y + (interface->getEchelle() * 20) + ((i % 9) * ecart)));
        liste_demandes_sauvegarde_row.push_back(r);
    }
}

void MenuJeu::drawTo(sf::RenderWindow &fenetre, sf::Time frameTime)
{
    timePopUp += frameTime.asSeconds();
    if (timePopUp > 0.8f)
    {
        update();
        timePopUp = 0;
    }
    if (initPartie)
    {
        if (interface->initBoard())
        {
            initPartie = false;
        }
    }

    fenetre.draw(spriteCadreMenu);
    fenetre.draw(spriteTableau);
    if (page_onglet == 0)
    {
        fenetre.draw(spriteSousTableau_petit);
        fenetre.draw(spriteSousTableau_grand);
        fenetre.draw(spriteChoixMode);

        fenetre.draw(mode);
        if (mode_de_jeu[indice_mode_de_jeu] == "Normal")
        {
            fenetre.draw(spriteChoixJoueur);
            fenetre.draw(nbrJoueur);
            fenetre.draw(nom_nbrJoueur);
        }
        fenetre.draw(nom_mode);
        for (auto &b : boutons1)
        {
            if (b->getNomBouton() == "nbrJoueur_next" || b->getNomBouton() == "nbrJoueur_previous")
            {
                if (mode_de_jeu[indice_mode_de_jeu] == "Normal")
                {
                    b->drawTo(fenetre);
                }
            }
            else
            {
                b->drawTo(fenetre);
            }
        }
        for (int i = 0; static_cast<size_t>(i) < liste_demandes_row.size(); i++)
        {
            if (i >= page_amis * 6 && i < (page_amis + 1) * 6)
            {
                liste_demandes_row[i]->drawTo(fenetre);
            }
        }
    }
    else if (page_onglet == 1)
    {
        fenetre.draw(spriteSousTableauXL);
        if (liste_demandes_partie_row.empty())
        {
            annonce.setString("Aucune invitation");
            annonce.setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + (spriteSousTableauXL.getGlobalBounds().width / 2.0) - (annonce.getGlobalBounds().width / 2.0), spriteSousTableauXL.getPosition().y + (spriteSousTableauXL.getGlobalBounds().height / 2.0) - (annonce.getGlobalBounds().height / 2.0) - 30));
            fenetre.draw(annonce);
        }
        else
        {
            for (int i = 0; static_cast<size_t>(i) < liste_demandes_partie_row.size(); i++)
            {
                if (i >= page_demande_partie * 9 && i < (page_demande_partie + 1) * 9)
                {
                    liste_demandes_partie_row[i]->drawTo(fenetre);
                }
            }
        }
    }
    else if (page_onglet == 2)
    {
        fenetre.draw(spriteSousTableauXL);
        if (liste_demandes_sauvegarde_row.empty())
        {
            annonce.setString("Aucune partie sauvegarde");
            annonce.setPosition(sf::Vector2f(spriteSousTableauXL.getPosition().x + (spriteSousTableauXL.getGlobalBounds().width / 2.0) - (annonce.getGlobalBounds().width / 2.0), spriteSousTableauXL.getPosition().y + (spriteSousTableauXL.getGlobalBounds().height / 2.0) - (annonce.getGlobalBounds().height / 2.0) - 30));
            fenetre.draw(annonce);
        }
        else
        {
            for (int i = 0; static_cast<size_t>(i) < liste_demandes_sauvegarde_row.size(); i++)
            {
                if (i >= page_demande_sauvegarde * 9 && i < (page_demande_sauvegarde + 1) * 9)
                {
                    liste_demandes_sauvegarde_row[i]->drawTo(fenetre);
                }
            }
        }
    }

    for (auto &b : bouton_generals)
    {

        b->drawTo(fenetre);
    }
    for (auto &b : boutons_onglets)
    {
        b->drawTo(fenetre);
    }
}

void MenuJeu::mouseMove(sf::Vector2i pos)
{
    for (auto &b : bouton_generals)
    {
        b->isMouseOver(pos.x, pos.y);
    }
    for (auto &b : boutons1)
    {
        if (!initPartie)
        {
            b->isMouseOver(pos.x, pos.y);
        }
    }
    for (auto &b : boutons_onglets)
    {
        b->isMouseOver(pos.x, pos.y);
    }
    for (auto &r : liste_demandes_row)
    {
        r->mouseMove(pos);
    }
    for (auto &r : liste_demandes_partie_row)
    {
        r->mouseMove(pos);
    }
    for (auto &r : liste_demandes_sauvegarde_row)
    {
        r->mouseMove(pos);
    }
}

void MenuJeu::mouseClick(sf::Vector2i pos, int &ecran_actuel)
{

    if (page_onglet == 0)
    {
        for (auto &b : boutons1)
        {
            if (b->contains(pos.x, pos.y))
            {
                if (b->getNomBouton() == "mode_next")
                {
                    indice_mode_de_jeu = (indice_mode_de_jeu + 1) % 3;
                    nom_mode.setString(mode_de_jeu[indice_mode_de_jeu]);
                }
                if (b->getNomBouton() == "mode_previous")
                {
                    indice_mode_de_jeu = indice_mode_de_jeu - 1;
                    if (indice_mode_de_jeu < 0)
                    {
                        indice_mode_de_jeu = 2;
                    }
                    nom_mode.setString(mode_de_jeu[indice_mode_de_jeu]);
                }
                if (b->getNomBouton() == "nbrJoueur_next")
                {
                    indice_nombre_joueur = (indice_nombre_joueur + 1) % 2;
                    nom_nbrJoueur.setString(nombre_joueurs[indice_nombre_joueur]);
                }
                if (b->getNomBouton() == "nbrJoueur_previous")
                {
                    indice_nombre_joueur = indice_nombre_joueur - 1;
                    if (indice_nombre_joueur < 0)
                    {
                        indice_nombre_joueur = 1;
                    }
                    nom_nbrJoueur.setString(nombre_joueurs[indice_nombre_joueur]);
                }

                if (b->getNomBouton() == "start" && !initPartie)
                {
                    client->sendMessage("gc");
                    if (client->getMessage() == "1")
                    {
                        if (mode_de_jeu[indice_mode_de_jeu] == "Tournoi" || static_cast<int>(joueurs_invite.size()) == stoi(nombre_joueurs[indice_nombre_joueur]) - 1)
                        {
                            bool invit_reussi = true;
                            for (auto &j : joueurs_invite)
                            {
                                std::string message = "gs";
                                message.append(j);
                                client->sendMessage(message);
                                if (client->getMessage() != "1")
                                {
                                    invit_reussi = false;
                                }
                            }
                            if (invit_reussi)
                            {
                                client->sendMessage(("gl" + mode_de_jeu[indice_mode_de_jeu] + "\n" + nombre_joueurs[indice_nombre_joueur]));
                                interface->setInitPartie(false);
                                std::cout << client->getMessage() << std::endl;
                                interface->showPopup("Attente de joueurs");
                                initPartie = true;
                                joueurs_invite.clear();
                                indice_mode_de_jeu = 0;
                                indice_nombre_joueur = 0;
                                for (auto &r : liste_demandes_row)
                                {
                                    for (auto &b : r->getBoutons())
                                    {
                                        if (b->getNomCase() == "Inviter")
                                        {
                                            b->setTextColor(sf::Color::White);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            interface->showPopup("Nombre de joueurs\n   non respecte", true);
                        }
                    }
                }
            }
        }
        for (auto &r : liste_demandes_row)
        {
            if (r->mouseClick(pos, ecran_actuel))
            {
                if (std::find(joueurs_invite.begin(), joueurs_invite.end(), r->getNomAmi()) != joueurs_invite.end())
                {
                    // remove(joueurs_invite.begin(), joueurs_invite.end(), r->getNomAmi());
                    joueurs_invite.erase(std::remove(joueurs_invite.begin(), joueurs_invite.end(), r->getNomAmi()), joueurs_invite.end());
                    for (auto &b : r->getBoutons())
                    {
                        if (b->getNomCase() == "Inviter")
                        {
                            b->setTextColor(sf::Color::White);
                        }
                    }
                }
                else
                {
                    joueurs_invite.push_back(r->getNomAmi());
                    for (auto &b : r->getBoutons())
                    {
                        if (b->getNomCase() == "Inviter")
                        {
                            b->setTextColor(sf::Color::Green);
                        }
                    }
                }
            }
        }
    }
    if (page_onglet == 1)
    {
        for (auto &r : liste_demandes_partie_row)
        {
            if (r->mouseClick(pos, ecran_actuel))
            {
                client->sendMessage("gj" + r->getNomAmi());
                if (client->getMessage()[0] == '1')
                {
                    interface->setInitPartie(false);
                    interface->showPopup("En attente");
                    interface->initBoard();
                    initPartie = true;
                }
                else
                {
                    interface->showPopup("Joueur non connecte", true);
                }
            }
        }
    }
    if (page_onglet == 2)
    {
        for (auto &r : liste_demandes_sauvegarde_row)
        {
            if (r->mouseClick(pos, ecran_actuel))
            {
                std::string nom = r->getNomAmi();
                // nom.erase(std::remove(nom.begin(), nom.end(), ' '), nom.end());
                client->sendMessage("gr" + nom);
                if (client->getMessage()[0] == '1')
                {
                    client->sendMessage("gl");
                    interface->setInitPartie(false);
                    interface->showPopup("En attente");
                    interface->initBoard();
                    initPartie = true;
                }
            }
        }
    }
    for (auto &b : bouton_generals)
    {
        if (b->contains(pos.x, pos.y))
        {
            if (b->getNomBouton() == "nextPage")
            {
                if (page_onglet == 0 && static_cast<int>(liste_demandes_row.size()) > (page_amis + 1) * 6)
                {
                    page_amis++;
                }
                else if (page_onglet == 1 && static_cast<int>(liste_demandes_partie_row.size()) > (page_demande_partie + 1) * 9)
                {
                    page_demande_partie++;
                }
                else if (page_onglet == 2 && static_cast<int>(liste_demandes_sauvegarde_row.size()) > (page_demande_sauvegarde + 1) * 9)
                {
                    page_demande_sauvegarde++;
                }
            }
            if (b->getNomBouton() == "previousPage")
            {
                if (page_onglet == 0 && page_amis > 0)
                {
                    page_amis--;
                }
                else if (page_onglet == 1 && page_demande_partie > 0)
                {
                    page_demande_partie--;
                }
                else if (page_onglet == 2 && page_demande_sauvegarde > 0)
                {
                    page_demande_sauvegarde--;
                }
            }
            if (b->getNomBouton() == "retour")
            {
                interface->hidePopup();
                page_amis = 0;
                page_onglet = 0;
                page_demande_partie = 0;
                page_demande_sauvegarde = 0;
                friendlist.clear();
                invitelist.clear();
                sauvegardelist.clear();
                joueurs_invite.clear();
                if (initPartie)
                {
                    client->sendMessage("gd");
                    if (client->getMessage() == "1")
                    {
                        ecran_actuel = 1;
                        interface->showPopup("La partie a ete annule !", true);
                        initPartie = false;
                    }
                }
                else
                {
                    ecran_actuel = 1;
                }
            }
        }
    }
    for (auto &b : boutons_onglets)
    {
        if (b->contains(pos.x, pos.y))
        {
            if (b->getNomBouton() == "onglet1")
            {
                parseConnectedFriends();
                page_onglet = 0;
            }
            if (b->getNomBouton() == "onglet2")
            {
                parseInvitation();
                page_onglet = 1;
            }
            if (b->getNomBouton() == "onglet3")
            {
                parseSaveInvit();
                page_onglet = 2;
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

void MenuJeu::update()
{
    parseConnectedFriends();
    parseInvitation();
    parseSaveInvit();
}

void MenuJeu::parseMessageClient(std::string s, std::vector<std::string> *liste)
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