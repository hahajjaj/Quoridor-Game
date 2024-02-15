#include "menu_ami.hpp"

MenuAmi::MenuAmi(float echelle, int width, int height, sf::Font *font_jeu, sf::Vector2f position, Client *client, unique_ptr<MenuChat> &menu_chat) : echelle(echelle), width(width), height(height), font_jeu(font_jeu), position(position), client(client), menu_chat(menu_chat)
{
    loadSprite();
}

MenuAmi::~MenuAmi()
{
}

void MenuAmi::loadSprite()
{
    cadre_menu.loadFromFile("ressources/menu_ami_assets/cadre.png");
    bouton_x.loadFromFile("ressources/menu_ami_assets/bouton_x.png");
    bouton_x_selec.loadFromFile("ressources/menu_ami_assets/bouton_x_selec.png");
    bouton_v.loadFromFile("ressources/menu_ami_assets/bouton_v.png");
    bouton_v_selec.loadFromFile("ressources/menu_ami_assets/bouton_v_selec.png");
    chat.loadFromFile("ressources/menu_ami_assets/chat.png");
    bouton_next.loadFromFile("ressources/menu_ami_assets/next.png");
    bouton_next_selec.loadFromFile("ressources/menu_ami_assets/next_selec.png");
    bouton_previous.loadFromFile("ressources/menu_ami_assets/previous.png");
    bouton_previous_selec.loadFromFile("ressources/menu_ami_assets/previous_selec.png");
    search.loadFromFile("ressources/menu_ami_assets/search.png");
    tableau1.loadFromFile("ressources/menu_ami_assets/tableau_1.png");
    tableau2.loadFromFile("ressources/menu_ami_assets/tableau_2.png");
    bouton_simple.loadFromFile("ressources/menu_assets/case_menu_principal.png");
    bouton_simple_selec.loadFromFile("ressources/menu_assets/case_menu_principal_transparent.png");

    spriteCadreMenu.setTexture(cadre_menu);
    spriteCadreMenu.scale((float)width / cadre_menu.getSize().x, (float)height / cadre_menu.getSize().y);

    spriteTableau1.setTexture(tableau1);
    spriteTableau1.scale(((float)spriteCadreMenu.getGlobalBounds().width - (echelle * 505)) / tableau1.getSize().x, ((float)spriteCadreMenu.getGlobalBounds().height - (echelle * 200)) / tableau1.getSize().y);

    spriteTableau2.setTexture(tableau2);
    spriteTableau2.scale(((float)spriteCadreMenu.getGlobalBounds().width - (echelle * 705)) / tableau2.getSize().x, ((float)spriteCadreMenu.getGlobalBounds().height - (echelle * 300)) / tableau2.getSize().y);

    search_friend = new Textbox(echelle, sf::Vector2f(315.0, 80.0), sf::Vector2f(position.x + (echelle * 45) + spriteTableau1.getGlobalBounds().width + (echelle * 20), position.y - (echelle * 20) + (height - spriteTableau2.getGlobalBounds().height - (echelle * 100)) / 2.0), sf::Color::White, sf::Color::Black, &search);
    search_friend->setLimit(true, 11);
    search_friend->setFont(*font_jeu);
    search_friend->setCharText(50);

    setPosition();

    text_tableau_1.setString("Amis");
    text_tableau_1.setFont(*font_jeu);
    text_tableau_1.setCharacterSize(echelle * 60);
    text_tableau_1.setPosition(spriteTableau1.getPosition().x + (spriteTableau1.getGlobalBounds().width / 2.0) - (text_tableau_1.getGlobalBounds().width / 2.0), spriteTableau1.getPosition().y - (echelle * 40));
    text_tableau_1.setFillColor(sf::Color::White);

    text_tableau_2.setString("Demandes");
    text_tableau_2.setCharacterSize(echelle * 40);
    text_tableau_2.setFont(*font_jeu);
    text_tableau_2.setPosition(spriteTableau2.getPosition().x + (spriteTableau2.getGlobalBounds().width / 2.0) - (text_tableau_2.getGlobalBounds().width / 2.0), spriteTableau2.getPosition().y - (echelle * 20));
    text_tableau_2.setFillColor(sf::Color::White);

    spriteSearch.setTexture(search);
    spriteSearch.scale(((float)spriteTableau2.getGlobalBounds().width - (echelle * 80)) / search.getSize().x, (echelle * 80.0) / search.getSize().y);

    Button *bouton_search = new Button(echelle, "", sf::Vector2f(80, 80), 30, font_jeu, &bouton_v, &bouton_v_selec);
    bouton_search->setPosition(sf::Vector2f(position.x + (echelle * 45) + spriteTableau1.getGlobalBounds().width + (echelle * 20) + spriteSearch.getGlobalBounds().width, position.y - (echelle * 20) + (height - spriteTableau2.getGlobalBounds().height - (echelle * 100)) / 2.0));
    bouton_search->setNomBouton("searchAmi");
    boutons.push_back(bouton_search);

    Button *button_next = new Button(echelle, "", sf::Vector2f(50, 50), 30, font_jeu, &bouton_next, &bouton_next_selec);
    button_next->setPosition(sf::Vector2f(position.x + (echelle * 45) + (echelle * 305), position.y + (echelle * 780)));
    button_next->setNomBouton("next");
    boutons.push_back(button_next);

    Button *button_prev = new Button(echelle, "", sf::Vector2f(50, 50), 30, font_jeu, &bouton_previous, &bouton_previous_selec);
    button_prev->setPosition(sf::Vector2f(position.x + (echelle * 45) + (echelle * 200), position.y + (echelle * 780)));
    button_prev->setNomBouton("previous");
    boutons.push_back(button_prev);

    Button *bouton_back = new Button(echelle, "Retour", sf::Vector2f(100, 40), 25, font_jeu, &bouton_simple, &bouton_simple_selec);
    bouton_back->setPosition(sf::Vector2f(position.x + (echelle * 30), position.y + (echelle * 820)));
    bouton_back->setId(1);
    boutons.push_back(bouton_back);
}

void MenuAmi::updateFriendList()
{
    float ecart = echelle * 50;
    liste_ami_row.clear();
    for (size_t i = 0; i < friendlist.size(); i++)
    {
        rowFriend *r = new rowFriend(echelle, friendlist[i], font_jeu, sf::Vector2f(position.x + (echelle * 80), position.y + (echelle * 110) + ((i % 13) * ecart)));
        liste_ami_row.push_back(r);
    }
}

void MenuAmi::updateRequestList()
{
    float ecart = echelle * 50;
    liste_demandes_row.clear();
    for (size_t i = 0; i < requestList.size(); i++)
    {
        rowRequest *r = new rowRequest(echelle, requestList[i], font_jeu, sf::Vector2f(spriteTableau2.getPosition().x + (echelle * 30), spriteTableau2.getPosition().y + (echelle * 30) + ((i % 13) * ecart)), 25, true);
        liste_demandes_row.push_back(r);
    }
}

void MenuAmi::setPosition()
{
    spriteCadreMenu.setPosition(position);

    spriteTableau1.setPosition(position.x + (echelle * 45), position.y - (echelle * 20) + (height - spriteTableau1.getGlobalBounds().height) / 2.0);

    spriteTableau2.setPosition(position.x + (echelle * 45) + spriteTableau1.getGlobalBounds().width + (echelle * 20), position.y - (echelle * 20) + (height - spriteTableau2.getGlobalBounds().height + (echelle * 100)) / 2.0);

    spriteSearch.setPosition(position.x + (echelle * 45) + spriteTableau1.getGlobalBounds().width + (echelle * 20), position.y + spriteTableau2.getPosition().y);
}

void MenuAmi::drawTo(sf::RenderWindow &fenetre, sf::Time frameTime)
{
    timePopUp += frameTime.asSeconds();
    if (timePopUp > 0.8f)
    {
        update();
        timePopUp = 0;
    }

    fenetre.draw(spriteCadreMenu);
    fenetre.draw(spriteTableau1);
    fenetre.draw(text_tableau_1);
    fenetre.draw(spriteTableau2);
    fenetre.draw(text_tableau_2);
    search_friend->drawTo(fenetre);
    for (auto &b : boutons)
    {
        b->drawTo(fenetre);
    }
    for (int i = 0; static_cast<size_t>(i) < liste_ami_row.size(); i++)
    {
        if (i >= page * 13 && i < (page + 1) * 13)
        {
            liste_ami_row[i]->drawTo(fenetre);
        }
    }
    for (int i = 0; static_cast<size_t>(i) < liste_demandes_row.size(); i++)
    {
        if (i < 11)
        {
            liste_demandes_row[i]->drawTo(fenetre);
        }
    }
}

void MenuAmi::mouseClick(sf::Vector2i pos, int &ecran_actuel)
{
    if (search_friend->isMouseOver(pos.x, pos.y))
    {
        search_friend->setSelected(true);
    }
    else
    {
        search_friend->setSelected(false);
    }
    for (auto &b : boutons)
    {
        if (b->contains(pos.x, pos.y))
        {
            if (b->getNomBouton() == "searchAmi")
            {
                client->sendMessage(("fi" + search_friend->getText()));
                if (client->getMessage()[0] == '1')
                {
                    search_friend->empty();
                }
            }
            if (b->getNomCase() == "Retour")
            {
                ecran_actuel = b->getId();
            }
            if (b->getNomBouton() == "next" && static_cast<int>(liste_ami_row.size()) > (page + 1) * 13)
            {
                page += 1;
            }
            if (b->getNomBouton() == "previous" && page > 0)
            {
                page -= 1;
            }
        }
    }
    for (auto &r : liste_ami_row)
    {
        for (auto &b : r->getBoutons())
        {
            if (b->contains(pos.x, pos.y))
            {
                if (b->getNomBouton() == "2")
                {
                    client->sendMessage(("fd" + r->getNomAmi()));
                    if (client->getMessage()[0] == '1')
                    {
                        friendlist.erase(std::remove(friendlist.begin(), friendlist.end(), r->getNomAmi()), friendlist.end());
                        updateFriendList();
                    }
                }
                else if (b->getNomBouton() == "1")
                {
                    menu_chat->setCorrespondant(r->getNomAmi());
                    ecran_actuel = 7;
                }
            }
        }
    }
    for (auto &r : liste_demandes_row)
    {
        for (auto &b : r->getBoutons())
        {
            if (b->contains(pos.x, pos.y))
            {
                if (b->getNomBouton() == "2")
                {
                    client->sendMessage(("fr" + r->getNomAmi()));
                    if (client->getMessage()[0] == '1')
                    {
                        requestList.erase(std::remove(requestList.begin(), requestList.end(), r->getNomAmi()), requestList.end());
                        updateRequestList();
                    }
                }
                else if (b->getNomBouton() == "1")
                {
                    client->sendMessage(("fa" + r->getNomAmi()));
                    if (client->getMessage()[0] == '1')
                    {
                        requestList.erase(std::remove(requestList.begin(), requestList.end(), r->getNomAmi()), requestList.end());
                        updateRequestList();
                    }
                }
            }
        }
    }
}

void MenuAmi::mouseMove(sf::Vector2i pos)
{
    for (auto &b : boutons)
    {
        b->isMouseOver(pos.x, pos.y);
    }
    for (auto &r : liste_ami_row)
    {
        r->mouseMove(pos);
    }
    for (auto &r : liste_demandes_row)
    {
        r->mouseMove(pos);
    }
}

void MenuAmi::textEntre(sf::Event evenement)
{
    search_friend->typedOn(evenement);
}

void MenuAmi::parseListeAmi(std::string s)
{
    s.erase(0, 1);
    friendlist.clear();
    std::string delimiter = "\n";
    auto start = 0U;
    auto end = s.find(delimiter);
    while (end != std::string::npos)
    {
        friendlist.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
}

void MenuAmi::parseRequestList(std::string s)
{
    s.erase(0, 1);
    requestList.clear();
    std::string delimiter = "\n";
    auto start = 0U;
    auto end = s.find(delimiter);
    while (end != std::string::npos)
    {
        requestList.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
}

void MenuAmi::update()
{
    client->sendMessage("fgf");
    std::string liste_amis = client->getMessage();
    parseListeAmi(liste_amis);
    updateFriendList();
    client->sendMessage("fgi");
    std::string liste_demandes = client->getMessage();
    parseRequestList(liste_demandes);
    updateRequestList();
}