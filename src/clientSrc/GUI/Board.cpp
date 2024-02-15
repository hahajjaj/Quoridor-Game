#include "Board.hpp"
#include "../Interface_GUI.hpp"

BoardGui::BoardGui(float echelle, sf::RenderWindow *window, int nombre_joueur, sf::Font *font, sf::Texture *cadre, int &ecran_actuel, Client *client, GUI *interface) : echelle(echelle), window{window}, nombre_joueur(nombre_joueur), font_jeu(font), cadre_menu(cadre), client(client), interface(interface)
{
    hauteur = hauteur * echelle;
    largeur = largeur * echelle;

    float cote_case = ((hauteur / taille_plateau) / (echelle * 100.0) * (echelle * 75.0));
    taille_case = sf::Vector2f(cote_case, cote_case);
    texture_map.loadFromFile("ressources/map4.png");
    map.setTexture(texture_map);
    map.scale((1300 * echelle) / texture_map.getSize().x, (1000.0 * echelle) / texture_map.getSize().y);
    map.setPosition(0, 0);
    loadTextures();
    loadBoard();
    initPion();
    loadMenuPause(ecran_actuel);
}

BoardGui::~BoardGui()
{
    std::cout << "deleting" << std::endl;
    for (auto &i : plateau)
    {
        for (auto &j : i)
        {
            delete j;
        }
        i.clear();
    }
    plateau.clear();

    for (auto &p : vecteur_pions)
    {
        delete p;
    }
    vecteur_pions.clear();

    for (auto &m : vecteur_murs)
    {
        delete m;
    }
    vecteur_murs.clear();

    delete bouton_de_pause;

    delete menu_pause;
}

void BoardGui::loadTextures()
{
    tunder1.loadFromFile("ressources/teleportation.png");
    explosion_texture.loadFromFile("ressources/explosion2.png");
    teleporte1 = new Animation();
    teleporte1->setSpriteSheet(tunder1);
    teleporte1->addFrame(sf::IntRect(0, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(48, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(96, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(144, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(192, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(240, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(288, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(336, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(384, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(432, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(480, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(528, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(576, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(624, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(672, 0, 48, 48));
    teleporte1->addFrame(sf::IntRect(720, 0, 48, 48));

    teleporte2 = new Animation();
    teleporte2->setSpriteSheet(tunder1);
    teleporte2->addFrame(sf::IntRect(720, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(672, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(624, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(576, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(528, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(480, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(432, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(384, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(336, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(288, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(240, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(192, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(144, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(96, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(48, 0, 48, 48));
    teleporte2->addFrame(sf::IntRect(0, 0, 48, 48));

    explosion = new Animation();
    explosion->setSpriteSheet(explosion_texture);
    explosion->addFrame(sf::IntRect(0, 0, 64, 64));
    explosion->addFrame(sf::IntRect(64, 0, 64, 64));
    explosion->addFrame(sf::IntRect(128, 0, 64, 64));
    explosion->addFrame(sf::IntRect(192, 0, 64, 64));
    explosion->addFrame(sf::IntRect(0, 64, 64, 64));
    explosion->addFrame(sf::IntRect(64, 64, 64, 64));
    explosion->addFrame(sf::IntRect(128, 64, 64, 64));
    explosion->addFrame(sf::IntRect(192, 64, 64, 64));
    explosion->addFrame(sf::IntRect(0, 128, 64, 64));
    explosion->addFrame(sf::IntRect(64, 128, 64, 64));
    explosion->addFrame(sf::IntRect(128, 128, 64, 64));
    explosion->addFrame(sf::IntRect(192, 128, 64, 64));
    explosion->addFrame(sf::IntRect(0, 192, 64, 64));
    explosion->addFrame(sf::IntRect(64, 192, 64, 64));
    explosion->addFrame(sf::IntRect(128, 192, 64, 64));
    explosion->addFrame(sf::IntRect(192, 192, 64, 64));

    animatedBombe = new AnimatedSprite(sf::seconds(0.05), false, false);
    animatedBombe->scale((echelle * 500.0) / 64, (echelle * 500.0) / 64);

    animatedSprite = new AnimatedSprite(sf::seconds(0.05), false, false);
    currentAnimation = teleporte1;
    animatedSprite->scale((echelle * 100.0) / 48, (echelle * 100.0) / 48.0);
}

void BoardGui::loadBoard()
{
    sf::Texture *texture_portal = new sf::Texture();
    // texture_portal->loadFromFile("ressources/portal_r.png");
    texture_portal->loadFromFile("ressources/Effect5.png");
    item1.loadFromFile("ressources/item1.png");
    item2.loadFromFile("ressources/item2.png");
    textures_items.push_back(item1);
    textures_items.push_back(item2);

    std::string alpha = "abcdefghi";
    for (int i = 0; i < taille_plateau; i++)
    {

        plateau.push_back({});
        for (int j = 0; j < taille_plateau; j++)
        {
            std::string nom_case = "";
            nom_case.push_back(alpha[i]);
            nom_case.append(to_string(j + 1));
            std::pair<int, int> pos_plat = {j, i};
            Case *c = new Case(echelle, sf::Vector2f((j * (hauteur / taille_plateau)) + (((hauteur / taille_plateau) - (taille_case.y)) / 2.0), (i * (largeur / taille_plateau)) + (((largeur / taille_plateau) - (taille_case.x)) / 2.0)), taille_case, window, pos_plat, nom_case, texture_portal);
            plateau[i].push_back(c);
        }
    }
    loadWall();
}

void BoardGui::loadMenuPause(int &ecran_actuel)
{
    // Image Cadre MenuGui
    cadre_menu->loadFromFile("ressources/menu_assets/cadre_menu.png");
    texture_bouton_pause.loadFromFile("ressources/menu_assets/pause2.png");
    texture_bouton_pause_selec.loadFromFile("ressources/menu_assets/pause2_transparent.png");
    bouton.loadFromFile("ressources/menu_jeu/bouton_petit.png");
    bouton_selec.loadFromFile("ressources/menu_jeu/bouton_petit_selec.png");
    bouton_bomb.loadFromFile("ressources/bouton_bomb.png");
    bouton_bomb_selec.loadFromFile("ressources/bouton_bomb_selec.png");
    
    eclair.setTexture(item1);
    eclair.scale((echelle * 50.0) / item1.getSize().x, (echelle * 50.0)/ item1.getSize().y);
    eclair.setPosition(echelle * 1050, echelle * 115);
    bouton_de_pause = new Button(echelle, "", sf::Vector2f(50, 50), 30, font_jeu, &texture_bouton_pause, &texture_bouton_pause_selec);
    bouton_de_pause->setPosition(sf::Vector2f(echelle * 1230, echelle * 10));
    std::vector<std::string> nom_bouton_pause{"Reprendre", "Demander une pause", "Quitter"};
    menu_pause = new MenuGui(echelle, "pause", 600, 600, ecran_actuel, nom_bouton_pause, sf::Vector2f((1300.0 / 2) - 300, (1000.0 / 2) - 300), font_jeu, cadre_menu);
    std::vector<int> ids{0, 1, 1};
    menu_pause->setIds(ids);

    bouton_retour = new Button(echelle, "Retour", sf::Vector2f(200, 50), 25, font_jeu, &bouton, &bouton_selec);
    bouton_retour->setPosition(sf::Vector2f(0 + (window->getSize().x / 2.0) - 100, 0 + (window->getSize().y - (window->getSize().y / 3.0))));

    bouton_bombe = new Button(echelle, "", sf::Vector2f(50, 50), 20, font_jeu, &bouton_bomb, &bouton_bomb_selec);
    bouton_bombe->setPosition(sf::Vector2f(echelle * 950, echelle * 400));
}

void BoardGui::draw(sf::Time frameTime)
{
    timePopUp += frameTime.asSeconds();
    std::string res;
    if (timePopUp > 0.5f)
    {
        res = client->getMessage(false);
        if (res[0] != '0')
        {
            std::cout << " test probleme :    " << res << std::endl;
        }
        timePopUp = 0;
    }
    if (res[0] == '2')
    {
        isFinished = true;
        res.erase(0, 1);
        // std::string res2 = client->getMessage();
        interface->setEcranActuel(6);
        // res2.erase(0, 1);
        interface->showPopup(res, true);
    }
    else if (res[0] == '3')
    {
        res.erase(0, 1);
        interface->showPopup(res);
        popUpTournoi = true;
    }
    else if (res != "" && res != "0" && res != ancien_etat)
    {
        if (popUpTournoi)
        {
            interface->hidePopup();
            popUpTournoi = false;
        }
        int indice_new_pos = res.find("m");
        int indice_old_pos = ancien_etat.find("m");
        if (res[indice_new_pos - 1] == ancien_etat[indice_old_pos - 1])
        {
            anime = true;
        }
        else
        {
            updateBoard(res);
        }
        currentAnimation = teleporte1;
        enum_animation = teleport1;
        sf::Vector2f pos2(vecteur_pions[currentJoueur_serveur]->getPosition().x - 30, vecteur_pions[currentJoueur_serveur]->getPosition().y - 30);
        setPositionAnimation(pos2);
        nouvel_etat = res;
    }
    if (joueur_client == currentJoueur_serveur)
    {
        isWaitingOpponent = false;
    }
    else
    {
        isWaitingOpponent = true;
    }

    window->draw(map);
    for (int y = 0; y < taille_plateau; y++)
    {
        for (int x = 0; x < taille_plateau; x++)
        {
            for (auto &pion : vecteur_pions)
            {

                if (plateau[y][x]->contains((sf::Vector2i)pion->getPosition()) && currentJoueur_serveur == pion->getId() && currentJoueur_serveur == joueur_client)
                {
                    plateau[y][x]->setColor(sf::Color(255, 55, 0, 160));
                }
                else
                {
                    plateau[y][x]->setColor(sf::Color(255, 255, 255, 10));
                }
                plateau[y][x]->draw(frameTime);
            }
        }
    }
    for (auto &w : vecteur_murs)
    {
        w->draw();
    }

    for (auto &p : vecteur_pions)
    {
        p->draw(frameTime);
    }
    for (auto &i : vecteur_items)
    {
        i->draw(frameTime);
    }
    for (int i = 0; static_cast<size_t>(i) < nombre_murs.size(); i++)
    {
        if (joueur_client == i)
        {
            window->draw(nombre_murs[i]);
        }
    }
    bouton_de_pause->drawTo(*window);
    if (pause)
    {
        menu_pause->drawTo(*window);
    }
    if (isFinished)
    {
        bouton_retour->drawTo(*window);
    }
    if (bombe)
    {
        bouton_bombe->drawTo(*window);
    }
    if(traverser){
        window->draw(eclair);
    }

    if (anime)
    {

        animatedSprite->play(*currentAnimation);
        animatedSprite->update(frameTime);
        if (enum_animation == teleport1 && animatedSprite->getCurrentFram() == 3)
        {
            vecteur_pions[currentJoueur_serveur]->setAfficher(false);
            // last_indice = currentJoueur_serveur;
        }
        if (enum_animation == teleport2 && animatedSprite->getCurrentFram() == 14)
        {
            vecteur_pions[last_indice]->setAfficher(true);
        }
        window->draw(*animatedSprite);
        if (!animatedSprite->isPlaying())
        {
            anime = false;
            updateBoard(nouvel_etat);
            animatedSprite->stop();
        }
        if (!anime && joueur_client == currentJoueur_serveur)
        {
            isWaitingOpponent = true;
            coup = "gm";
        }
    }
    if (anime_bomb)
    {
        drawBomb(frameTime);
    }
}

void BoardGui::drawBomb(sf::Time frameTime)
{
    animatedBombe->play(*explosion);
    animatedBombe->update(frameTime);
    window->draw(*animatedBombe);
    if (!animatedBombe->isPlaying())
    {
        anime_bomb = false;
        animatedBombe->stop();
    }
    if (!anime_bomb && joueur_client == currentJoueur_serveur)
    {
        isWaitingOpponent = true;
        coup = "gm";
    }
}

void BoardGui::loadWall()
{
    sf::Vector2f taille_mur_ver((((hauteur / taille_plateau) - (taille_case.y))), taille_case.y);
    sf::Vector2f taille_mur_hor(taille_case.x, (((hauteur / taille_plateau) - (taille_case.y))));
    for (int y = 0; y < taille_plateau; y++)
    {
        for (int x = 0; x < taille_plateau; x++)
        {
            sf::Vector2f position_wall_hor(plateau[y][x]->getPosition().x, (plateau[y][x]->getPosition().y + taille_case.x));
            sf::Vector2f position_wall_ver(plateau[y][x]->getPosition().x + taille_case.x, (plateau[y][x]->getPosition().y));
            if (x != taille_plateau - 1)
            {
                std::pair<std::pair<int, int>, std::pair<int, int>> case_environs;
                case_environs.first.first = x;
                case_environs.first.second = y;
                case_environs.second.first = x + 1;
                case_environs.second.second = y;
                Wall *w2 = new Wall(position_wall_ver, taille_mur_ver, window, case_environs, false);
                vecteur_murs.push_back(w2);
            }
            if (y != taille_plateau - 1)
            {
                std::pair<std::pair<int, int>, std::pair<int, int>> case_environs;
                case_environs.first.first = x;
                case_environs.first.second = y;
                case_environs.second.first = x;
                case_environs.second.second = y + 1;
                Wall *w1 = new Wall(position_wall_hor, taille_mur_hor, window, case_environs, true);
                vecteur_murs.push_back(w1);
            }
        }
    }
}

void BoardGui::setPositionAnimation(sf::Vector2f pos2)
{

    animatedSprite->setPosition(pos2);
}

Wall *BoardGui::getMurVoisin(Wall *w)
{
    // si horizontal
    if (w->getOrientation())
    {
        for (auto &m : vecteur_murs)
        {
            if ((m->getCasesEnvironnante().first.first == w->getCasesEnvironnante().first.first + 1 && m->getCasesEnvironnante().first.second == w->getCasesEnvironnante().first.second) || (m->getCasesEnvironnante().first.first == w->getCasesEnvironnante().second.first + 1 && m->getCasesEnvironnante().first.second == w->getCasesEnvironnante().second.second))
            {
                if ((m->getCasesEnvironnante().second.first == w->getCasesEnvironnante().first.first + 1 && m->getCasesEnvironnante().second.second == w->getCasesEnvironnante().first.second) || (m->getCasesEnvironnante().second.first == w->getCasesEnvironnante().second.first + 1 && m->getCasesEnvironnante().second.second == w->getCasesEnvironnante().second.second))
                {
                    return m;
                }
            }
        }
    }
    else
    { // si vertical
        for (auto &m : vecteur_murs)
        {
            if ((m->getCasesEnvironnante().first.first == w->getCasesEnvironnante().first.first && m->getCasesEnvironnante().first.second == w->getCasesEnvironnante().first.second + 1) || (m->getCasesEnvironnante().first.first == w->getCasesEnvironnante().second.first && m->getCasesEnvironnante().first.second == w->getCasesEnvironnante().second.second + 1))
            {
                if ((m->getCasesEnvironnante().second.first == w->getCasesEnvironnante().first.first && m->getCasesEnvironnante().second.second == w->getCasesEnvironnante().first.second + 1) || (m->getCasesEnvironnante().second.first == w->getCasesEnvironnante().second.first && m->getCasesEnvironnante().second.second == w->getCasesEnvironnante().second.second + 1))
                {
                    return m;
                }
            }
        }
    }
    return nullptr;
}

void BoardGui::updtate(sf::Vector2i position, int &ecran_actuel)
{
    // if(!anime){
    //     currentAnimation = teleporte1;
    // }

    if (isFinished)
    {
        if (bouton_retour->contains(position.x, position.y))
        {
            isFinished = false;
            interface->hidePopup();
            ecran_actuel = 1;
        }
    }
    else if (!pause)
    {
        Case *c = nullptr;
        for (int y = 0; y < taille_plateau; y++)
        {
            for (int x = 0; x < taille_plateau; x++)
            {
                for (auto &p : vecteur_pions)
                {
                    if (!anime && p->getId() == joueur_client && !isWaitingOpponent && coup == "gm" && (plateau[y][x]->contains((sf::Vector2i)p->getPosition()) && plateau[y][x]->contains(position)))
                    {

                        click_case = !click_case;
                        if (click_case)
                        {
                            c = plateau[y][x];
                            coup.append("p");
                            coup.append(c->getNomCase());
                        }
                    }
                    else if (plateau[y][x]->contains(position) && plateau[y][x]->getClick())
                    {
                        plateau[y][x]->mouseClick(position);
                        coup.append(plateau[y][x]->getNomCase());
                        client->sendMessage(coup);
                    }
                }
            }
        }
        if (click_case && c != nullptr)
        {
            std::vector<Case *> voisins = getNeighbour(Point<>{c->getPosPlateau().first, c->getPosPlateau().second});
            for (auto &c : voisins)
            {
                c->setClick(true);
            }
            click_case = !click_case;
        }
        else
        {
            for (auto &v : plateau)
            {
                for (auto &c : v)
                {
                    if (c->getClick())
                    {
                        c->setClick(false);
                        coup = "gm";
                    }
                }
            }
        }
        for (auto &w : vecteur_murs)
        {
            if (!anime && w->contains(position) && !isWaitingOpponent)
            {
                std::pair<std::pair<int, int>, std::pair<int, int>> c = w->getCasesEnvironnante();
                std::string coup_mur;
                if (!bombe_is_click)
                {
                    coup_mur = "gmm";
                }
                else
                {
                    coup_mur = "gms";
                }
                coup_mur.append(plateau[c.first.second][c.first.first]->getNomCase());
                coup_mur.append(plateau[c.second.second][c.second.first]->getNomCase());
                client->sendMessage(coup_mur);
                std::string res = client->getMessage();
                if (res != "" && res != "0" && res != ancien_etat)
                {
                    if (bombe_is_click)
                    {
                        bombe_is_click = false;
                        bombe = false;
                        anime_bomb = true;
                        animatedBombe->setPosition(plateau[c.first.second][c.first.first]->getPosition().x - 200, plateau[c.first.second][c.first.first]->getPosition().y - 200);
                    }
                    updateBoard(res);
                    isWaitingOpponent = true;
                }
            }
        }
        if (bouton_de_pause->contains(position.x, position.y))
        {
            pause = true;
        }
        if (bombe)
        {
            if (bouton_bombe->contains(position.x, position.y))
            {
                bombe_is_click = !bombe_is_click;
                if (bombe_is_click)
                {
                    for (auto &p : plateau)
                    {
                        for (auto &c : p)
                        {
                            c->setColor(sf::Color(sf::Color(255, 87, 51, 150)));
                        }
                    }
                }
                else
                {
                    for (auto &p : plateau)
                    {
                        for (auto &c : p)
                        {
                            c->setColor(sf::Color(255, 255, 255, 10));
                        }
                    }
                }
            }
            else
            {
                bombe_is_click = false;
                for (auto &p : plateau)
                {
                    for (auto &c : p)
                    {
                        c->setColor(sf::Color(255, 255, 255, 10));
                    }
                }
            }
        }
    }
    else
    {
        for (auto &b : *menu_pause->getBoutonsMenu())
        {
            if (b->contains(position.x, position.y))
            {
                if (b->getNomCase() == "Reprendre")
                {
                    pause = false;
                }
                else if (b->getNomCase() == "Demander une pause")
                {
                    if (currentJoueur_serveur == joueur_client && !mode_item)
                    {
                        client->sendMessage("gpause");
                        if (client->getMessage()[0] != '0')
                        {
                            pause = false;
                            ecran_actuel = b->getId();
                        }
                    }
                    else
                    {
                        if(mode_item){
                            interface->showPopup("Vous ne pouvez pas en\n     mode item", true);
                        }else{
                            interface->showPopup("Attendez votre tour", true);
                        }
                    }
                }
                else if (b->getNomCase() == "Quitter")
                {
                    client->sendMessage("gf");
                    std::string res = client->getMessage();
                    if (res[0] == '2')
                    {
                        res.erase(0, 1);
                        interface->showPopup(res, true);
                        ecran_actuel = b->getId();
                    }
                }
            }
        }
    }
}

bool BoardGui::respecteDelimitation(int y, int x)
{
    return ((y >= 0 && y < taille_plateau) && (x >= 0 && x < taille_plateau));
}

std::vector<Case *> BoardGui::getNeighbour(Point<> pos_init)
{
    std::vector<Case *> case_selectionne;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (!(i == 0 && j == 0) && (abs(i) != abs(j)) && respecteDelimitation(pos_init.y + j, pos_init.x + i))
            {
                case_selectionne.push_back(plateau[pos_init.y + j][pos_init.x + i]);
            }
        }
    }
    return case_selectionne;
}

void BoardGui::resetWallDraw()
{
    for (auto &w : vecteur_murs)
    {
        w->setAfficher(false);
    }
}

void BoardGui::updateBoard(std::string etatPlateau)
{
    traverser = false;
    for (auto &i : vecteur_items)
    {
        delete i;
    }
    vecteur_items.clear();
    nombre_murs.clear();

    ancien_etat = etatPlateau;
    last_indice = currentJoueur_serveur;
    etatPlateau.erase(0, 1);
    std::string buff = "";
    const char *info = etatPlateau.c_str();

    // pour les pions
    for (int i = 0; i < 4; ++i)
    {

        for (int nbPion = *info++ - '0'; nbPion > 0; --nbPion)
        {
            vecteur_pions[i]->update(sf::Vector2f(static_cast<float>(*info - '0'), static_cast<float>(*(info + 1) - '0')));
            info += 2;
        }
    }

    // pour les murs
    for (int i = 0; i < 4; ++i)
    {
        buff.clear();
        while (*info != 'n')
        {
            buff += *info++;
        }
        ++info;
        sf::Text nb(buff, *font_jeu, echelle * 60);
        nb.setPosition(echelle * 1125, echelle * 110);
        nombre_murs.push_back(nb);
    }
    buff.clear();

    while (*info != 'm')
        buff += *info++;
    ++info;
    // recupere les positions des murs déjà posé sur le plateau
    resetWallDraw();
    for (int nbWall = stoi(buff); nbWall > 0; --nbWall)
    {
        std::pair<int, int> case1 = {static_cast<int>(*info - '0'), static_cast<int>(*(info + 1) - '0')};
        info += 2;
        std::pair<int, int> case2 = {static_cast<int>(*info - '0'), static_cast<int>(*(info + 1) - '0')};
        info += 2;
        for (auto &w : vecteur_murs)
        {
            if (w->isMur(case1, case2))
            {
                if (!w->getAfficher())
                {
                    w->toggleAfficher();
                }
            }
        }
    }
    // recupere les positions des items sur le plateau
    buff.clear();
    while (*info != 'i')
        buff += *info++;
    ++info; // recupere le nb d'items
    for (int nbItem = stoi(buff); nbItem > 0; --nbItem)
    {
        sf::Vector2f pos = {static_cast<float>(*info - '0'), static_cast<float>(*(info + 1) - '0')};
        info += 2;
        // std::cout << "position : " << pos.x << " " << pos.y << std::endl;
        // info++;
        Item *i = new Item(echelle, window, pos, *info, &textures_items);
        vecteur_items.push_back(i);
        ++info;
    }

    // recupere le nb d'items pour chaque joueur
    for (int i = 0; i < 4; ++i)
    {
        buff.clear();
        while (*info != 'p')
            buff += *info++;
        ++info;
        for (int nbItem = stoi(buff); nbItem > 0; --nbItem)
        {
            // iRemaining[i].push_back(item[static_cast<int>(*info++ == 't')]);
            char it = *info++;
            if (it == 's' && joueur_client == i)
            {
                bombe = true;
            }
            else if (it == 't' && joueur_client == i)
            {
                traverser = true;
            }
        }
    }

    currentJoueur_serveur = *info++ - '0' - 1;
    int mode = *info++ - '0';
    joueur_client = *info++ - '0'; // id du joueur

    if (mode == 1)
    {
        mode_item = true;
    }

    for (auto &p : vecteur_pions)
    {
        if (!p->getAfficher())
        {
            sf::Vector2f pos2(vecteur_pions[last_indice]->getPosition().x - 30, vecteur_pions[last_indice]->getPosition().y - 30);
            setPositionAnimation(pos2);
            currentAnimation = teleporte2;
            enum_animation = teleport2;
            anime = true;
        }
    }
}

void BoardGui::initPion()
{
    for (int i = 0; i < nombre_joueur; i++)
    {
        Pion *p = new Pion{echelle, window, taille_case, i};
        vecteur_pions.push_back(p);
    }
}

void BoardGui::mouseMove(sf::Vector2i p)
{
    if (isFinished)
    {
        bouton_retour->isMouseOver(p.x, p.y);
    }
    else if (!pause)
    {
        if (!bombe_is_click)
        {
            for (int y = 0; y < taille_plateau; y++)
            {
                for (int x = 0; x < taille_plateau; x++)
                {
                    // for(auto &pion: vecteur_pions){
                    // if(plateau[y][x]->contains((sf::Vector2i)pion->getPosition()) && currentJoueur_serveur == pion->getId()){
                    //     plateau[y][x]->setColor(sf::Color(255,55,0,255));
                    //     plateau[y][x]->mouseMove(p);

                    // }else{
                    //     plateau[y][x]->setColor(sf::Color(255,255,255,10));
                    plateau[y][x]->mouseMove(p);
                    // }
                    // }
                }
            }
        }
        Wall *w2 = nullptr;
        for (auto &w : vecteur_murs)
        {
            if (w->mouseMove(p, bombe_is_click))
            {

                w2 = getMurVoisin(w);
            }
        }
        if (w2 != nullptr && !bombe_is_click)
        {
            w2->mouseMove(sf::Vector2i(w2->getPosition().x + 1, w2->getPosition().y + 1), false);
        }

        bouton_de_pause->isMouseOver(p.x, p.y);
        if (bombe)
        {
            bouton_bombe->isMouseOver(p.x, p.y);
        }
    }

    else
    {
        menu_pause->mouseMove(p);
    }
}