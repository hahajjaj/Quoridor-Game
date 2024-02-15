#include "menuChat.hpp"

MenuChat::MenuChat(float echelle, int width, int height, sf::Vector2f position, sf::Font *font_jeu, Client *client) : echelle(echelle), width(width), height(height), position(position), font_jeu(font_jeu), client(client)
{
    loadSprite();
    setPosition();
    loadButtons();
    nom_ami.setFont(*font_jeu);
    nom_ami.setCharacterSize(echelle * 30);
    nom_ami.setOutlineThickness(echelle * 2);
}

MenuChat::~MenuChat()
{
}

bool MenuChat::setCorrespondant(std::string nom)
{
    ami = nom;
    nom_ami.setString(ami);
    client->sendMessage(("mg" + ami));
    std::string chatl = client->getMessage();
    if (chatl[0] == '1')
    {
        parseChat(chatl);
        initLines();
        refresh = true;
        return true;
    }
    else
    {
        return false;
    }
}

void MenuChat::update(std::string s)
{
    parseChat(s);
    initLines();
}

void MenuChat::initLines()
{
    for (auto &s : chatlog)
    {
        sf::Text *l = new sf::Text(s, *font_jeu, echelle * 25);
        l->setOutlineThickness(1 * echelle);
        lines.push_back(l);
    }
    int j = 12;
    for (int i = lines.size() - 1; i >= 0; i--)
    {
        if (j != 0)
        {
            lines[i]->setPosition(spriteSousTableau1.getPosition().x + (echelle * 25), spriteSousTableau1.getPosition().y  + (j * 30 * echelle));
            j--;
        }
    }
}

void MenuChat::parseChat(std::string c)
{
    c.erase(0, 1);
    lines.clear();
    chatlog.clear();
    std::string delimiter = "\n";
    auto start = 0U;
    auto end = c.find(delimiter);
    while (end != std::string::npos)
    {
        chatlog.push_back(c.substr(start, end - start));
        start = end + delimiter.length();
        end = c.find(delimiter, start);
    }
    for (auto &s : chatlog)
    {
        std::cout << s << std::endl;
    }
}

void MenuChat::loadButtons()
{
    Button *bouton_send = new Button(echelle, "", sf::Vector2f(send_b.getSize().x * 2.8, send_b.getSize().y * 2.8), 30, font_jeu, &send_b, &send_selec);
    bouton_send->setPosition(sf::Vector2f(spriteCadreMenu.getPosition().x + (spriteCadreMenu.getGlobalBounds().width / 2.0) + (barre_chat.getSize().x * 2.8 * echelle) / 2.0 - ((send_b.getSize().x * 2.8 * echelle) / 2.0), spriteCadreMenu.getPosition().y + (spriteCadreMenu.getGlobalBounds().height / 2.0) + (spriteTableau.getGlobalBounds().height / 2.0) - ((barre_chat.getSize().y * 2.8 * echelle) / 2.0)));
    bouton_send->setNomBouton("send");
    boutons.push_back(bouton_send);

    Button *bouton_retour = new Button(echelle, "Retour", sf::Vector2f(bouton.getSize().x * 2.8, bouton.getSize().y * 2.8), 30, font_jeu, &bouton, &bouton_selec);
    bouton_retour->setPosition(sf::Vector2f(spriteCadreMenu.getPosition().x + 20, spriteCadreMenu.getPosition().y + spriteCadreMenu.getGlobalBounds().height + 10));
    bouton_retour->setNomBouton("retour");
    boutons.push_back(bouton_retour);

    message->setPosition(sf::Vector2f(spriteCadreMenu.getPosition().x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (send_b.getSize().x * 2.8 * echelle) / 2.0 - ((barre_chat.getSize().x * 2.8 * echelle) / 2.0), spriteCadreMenu.getPosition().y + (spriteCadreMenu.getGlobalBounds().height / 2.0) + (spriteTableau.getGlobalBounds().height / 2.0) - ((barre_chat.getSize().y * 2.8 * echelle) / 2.0)));
}

void MenuChat::loadSprite()
{
    cadre_menu.loadFromFile("ressources/menu_chat_assets/cadre.png");
    tableau.loadFromFile("ressources/menu_chat_assets/cadre_principal.png");
    sousTableau1.loadFromFile("ressources/menu_chat_assets/cadre_message.png");
    sousTableau2.loadFromFile("ressources/menu_chat_assets/cadre_profil.png");
    barre_chat.loadFromFile("ressources/menu_chat_assets/barre_chat.png");
    send_b.loadFromFile("ressources/menu_chat_assets/bouton.png");
    send_selec.loadFromFile("ressources/menu_chat_assets/bouton_selec.png");
    bouton.loadFromFile("ressources/menu_jeu/bouton_petit.png");
    bouton_selec.loadFromFile("ressources/menu_jeu/bouton_petit_selec.png");

    spriteCadreMenu.setTexture(cadre_menu);
    spriteTableau.setTexture(tableau);
    spriteSousTableau1.setTexture(sousTableau1);
    spriteSousTableau2.setTexture(sousTableau2);

    spriteCadreMenu.scale((((float)cadre_menu.getSize().x * 2.8) * echelle) / cadre_menu.getSize().x, ((float)cadre_menu.getSize().y * 2.8 * echelle) / cadre_menu.getSize().y);
    spriteTableau.scale(((float)tableau.getSize().x * 2.8 * echelle) / tableau.getSize().x, ((float)tableau.getSize().y * 2.8 * echelle) / tableau.getSize().y);
    spriteSousTableau1.scale(((float)sousTableau1.getSize().x * 2.8 * echelle) / sousTableau1.getSize().x, ((float)sousTableau1.getSize().y * 2.8 * echelle) / sousTableau1.getSize().y);
    spriteSousTableau2.scale(((float)sousTableau2.getSize().x * 2.8 * echelle) / sousTableau2.getSize().x, ((float)sousTableau2.getSize().y * 2.8 * echelle) / sousTableau2.getSize().y);

    message = new Textbox(echelle, sf::Vector2f(barre_chat.getSize().x * 2.8, barre_chat.getSize().y * 2.8), sf::Vector2f(0, 0), sf::Color::White, sf::Color::Black, &barre_chat, false);
    message->setLimit(true, 45);
    message->setFont(*font_jeu);
    message->setCharText(30);
}

void MenuChat::setPosition()
{
    spriteCadreMenu.setPosition(position);
    spriteTableau.setPosition(position.x + (spriteCadreMenu.getGlobalBounds().width / 2.0) - (spriteTableau.getGlobalBounds().width / 2.0), position.y + (spriteCadreMenu.getGlobalBounds().height / 2.0) - (spriteTableau.getGlobalBounds().height / 2.0) - (barre_chat.getSize().y * 2.8));
    spriteSousTableau1.setPosition(spriteTableau.getPosition().x + (spriteTableau.getGlobalBounds().width / 2.0) - spriteSousTableau2.getGlobalBounds().width / 2.0 - (spriteSousTableau1.getGlobalBounds().width / 2.0), spriteTableau.getPosition().y + (spriteTableau.getGlobalBounds().height / 2.0) - (spriteSousTableau1.getGlobalBounds().height / 2.0));
    spriteSousTableau2.setPosition(spriteTableau.getPosition().x + (spriteTableau.getGlobalBounds().width / 2.0) + spriteSousTableau1.getGlobalBounds().width / 2.0 - (spriteSousTableau2.getGlobalBounds().width / 2.0), spriteTableau.getPosition().y + (spriteTableau.getGlobalBounds().height / 2.0) - (spriteSousTableau1.getGlobalBounds().height / 2.0));
    nom_ami.setPosition(spriteSousTableau2.getPosition().x + (echelle * 30), spriteSousTableau2.getPosition().y + (echelle * 5));
}

void MenuChat::drawTo(sf::RenderWindow &fenetre)
{
    std::string res = client->getMessage(false);
    std::cout << res << std::endl;
    if (res[0] != '0')
    {
        update(res);
    }
    fenetre.draw(spriteCadreMenu);
    fenetre.draw(spriteTableau);
    fenetre.draw(spriteSousTableau1);
    fenetre.draw(spriteSousTableau2);
    message->drawTo(fenetre);
    for (auto &b : boutons)
    {
        b->drawTo(fenetre);
    }
    int j = 0;
    for (int i = lines.size() - 1; i >= 0; i--)
    {
        fenetre.draw(*lines[i]);
        j++;
        if (j == 12)
        {
            break;
        }
    }
    fenetre.draw(nom_ami);
}

void MenuChat::mouseMove(sf::Vector2i pos)
{
    for (auto &b : boutons)
    {
        b->isMouseOver(pos.x, pos.y);
    }
}

void MenuChat::mouseClick(sf::Vector2i pos, int &ecran_actuel)
{
    if (message->isMouseOver(pos.x, pos.y))
    {
        message->setSelected(true);
    }
    else
    {
        message->setSelected(false);
    }
    for (auto &b : boutons)
    {
        if (b->contains(pos.x, pos.y))
        {
            if (b->getNomBouton() == "send")
            {
                client->sendMessage(("ms" + message->getText()));
                message->empty();
            }
            if (b->getNomBouton() == "retour")
            {
                ecran_actuel = 5;
            }
        }
    }
}

void MenuChat::textEntre(sf::Event evenement)
{
    int charTyped = evenement.text.unicode;
    if (charTyped == ENTER_KEY)
    {
        client->sendMessage(("ms" + message->getText()));
        message->empty();
    }
    else
    {
        message->typedOn(evenement);
    }
}
