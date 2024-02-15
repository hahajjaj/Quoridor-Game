#include "LoginGUI.hpp"
#include "../../Interface_GUI.hpp"

LoginGUI::LoginGUI(float echelle, int width, int height, sf::Font *font_jeu, Client *client, unique_ptr<MenuAmi> &menu_ami, GUI *interface) : echelle(echelle), width(width), height(height), font_jeu(font_jeu), client(client), menu_ami(menu_ami), interface(interface)
{
    loadSprite();
}

LoginGUI::~LoginGUI()
{
}

void LoginGUI::loadSprite()
{
    cadre_petit.loadFromFile("ressources/menu_login_assets/cadre_petit.png");
    cadre_grand.loadFromFile("ressources/menu_login_assets/cadre_grand.png");
    bouton.loadFromFile("ressources/menu_login_assets/bouton.png");
    bouton_selec.loadFromFile("ressources/menu_login_assets/bouton_selec.png");
    bouton_2.loadFromFile("ressources/menu_login_assets/bouton_2.png");
    bouton_2_selec.loadFromFile("ressources/menu_login_assets/bouton_2_selec.png");
    bar_text.loadFromFile("ressources/menu_login_assets/bar_text.png");

    sprite_cadre_petit.setTexture(cadre_petit);
    sprite_cadre_petit.scale((float)(echelle * 400) / cadre_petit.getSize().x, (float)(echelle * 360) / cadre_petit.getSize().y);
    sprite_cadre_petit.setPosition(((float)width / 2.0) - (sprite_cadre_petit.getGlobalBounds().width / 2.0), ((float)height / 2.0) - (sprite_cadre_petit.getGlobalBounds().height / 2.0));

    sprite_cadre_grand.setTexture(cadre_grand);
    sprite_cadre_grand.scale((float)(echelle * 480) / cadre_grand.getSize().x, (float)(echelle * 480) / cadre_grand.getSize().y);
    sprite_cadre_grand.setPosition(((float)width / 2.0) - (sprite_cadre_grand.getGlobalBounds().width / 2.0), ((float)height / 2.0) - (sprite_cadre_grand.getGlobalBounds().height / 2.0));

    username = new Textbox(echelle, sf::Vector2f(380, 80), sf::Vector2f(sprite_cadre_grand.getPosition().x + (echelle * 50), sprite_cadre_grand.getPosition().y + (echelle * 80)), sf::Color::White, sf::Color::Black, &bar_text);
    username->setLimit(true, 10);
    username->setFont(*font_jeu);

    password = new Textbox(echelle, sf::Vector2f(380, 80), sf::Vector2f(sprite_cadre_grand.getPosition().x + (echelle * 50), sprite_cadre_grand.getPosition().y + (echelle * 80) + (echelle * 80) + (echelle * 50)), sf::Color::White, sf::Color::Black, &bar_text, true);
    password->setLimit(true, 10);
    password->setFont(*font_jeu);

    bouton_connection = new Button(echelle, "Se connecter", sf::Vector2f(400, 80), 30, font_jeu, &bouton_2, &bouton_2_selec);
    bouton_connection->setPosition(sf::Vector2f(sprite_cadre_grand.getPosition().x + (echelle * 40), sprite_cadre_grand.getPosition().y + (echelle * 80) + (echelle * 80) + (echelle * 50) + (echelle * 80) + (echelle * 40)));
    bouton_connection->setNomBouton("connection");
    boutons.push_back(bouton_connection);

    bouton_inscription = new Button(echelle, "S'inscrire", sf::Vector2f(400, 100), 30, font_jeu, &bouton_2, &bouton_2_selec);
    bouton_inscription->setPosition(sf::Vector2f(sprite_cadre_grand.getPosition().x + (echelle * 40), sprite_cadre_grand.getPosition().y + (echelle * 80) + (echelle * 80) + (echelle * 50) + (echelle * 80) + (echelle * 40)));
    bouton_inscription->setNomBouton("inscription");
    boutons.push_back(bouton_inscription);

    bouton_login = new Button(echelle, "Login", sf::Vector2f(320, 80), 30, font_jeu, &bouton, &bouton_selec);
    bouton_login->setPosition(sf::Vector2f(sprite_cadre_petit.getPosition().x + (echelle * 40), sprite_cadre_petit.getPosition().y + (echelle * 90)));
    bouton_login->setNomBouton("login");
    boutons.push_back(bouton_login);

    bouton_signIn = new Button(echelle, "Sign In", sf::Vector2f(320, 80), 30, font_jeu, &bouton, &bouton_selec);
    bouton_signIn->setPosition(sf::Vector2f(sprite_cadre_petit.getPosition().x + (echelle * 40), sprite_cadre_petit.getPosition().y + (echelle * 90) + (echelle * 80) + (echelle * 20)));
    bouton_signIn->setNomBouton("SignIn");
    boutons.push_back(bouton_signIn);

    bouton_retour = new Button(echelle, "Retour", sf::Vector2f(320, 80), 30, font_jeu, &bouton, &bouton_selec);
    bouton_retour->setPosition(sf::Vector2f(sprite_cadre_petit.getPosition().x + (echelle * 40), sprite_cadre_grand.getPosition().y + sprite_cadre_grand.getGlobalBounds().height + (echelle * 20)));
    bouton_retour->setNomBouton("retour");
    boutons.push_back(bouton_retour);
}

void LoginGUI::setPosition()
{
}

void LoginGUI::drawTo(sf::RenderWindow &fenetre)
{
    if (ecran_login)
    {
        fenetre.draw(sprite_cadre_petit);
        bouton_login->drawTo(fenetre);
        bouton_signIn->drawTo(fenetre);
    }
    else
    {
        fenetre.draw(sprite_cadre_grand);
        username->drawTo(fenetre);
        password->drawTo(fenetre);
        if (mode_login)
        {
            bouton_connection->drawTo(fenetre);
        }
        else
        {
            bouton_inscription->drawTo(fenetre);
        }
        bouton_retour->drawTo(fenetre);
    }
}

void LoginGUI::mouseMove(sf::Vector2i pos)
{
    if (ecran_login)
    {
        bouton_login->isMouseOver(pos.x, pos.y);
        bouton_signIn->isMouseOver(pos.x, pos.y);
    }
    else
    {
        if (mode_login)
        {
            bouton_connection->isMouseOver(pos.x, pos.y);
        }
        else
        {
            bouton_inscription->isMouseOver(pos.x, pos.y);
        }
        bouton_retour->isMouseOver(pos.x, pos.y);
    }
}

void LoginGUI::mouseClick(sf::Vector2i pos, int &ecran_actuel)
{
    if (username->isMouseOver(pos.x, pos.y))
    {
        username->setSelected(true);
    }
    else
    {
        username->setSelected(false);
    }
    if (password->isMouseOver(pos.x, pos.y))
    {
        password->setSelected(true);
    }
    else
    {
        password->setSelected(false);
    }

    if (ecran_login)
    {
        if (bouton_login->isMouseOver(pos.x, pos.y))
        {
            if (bouton_login->getNomBouton() == "login")
            {
                mode_login = true;
                ecran_login = false;
            }
        }
        if (bouton_signIn->isMouseOver(pos.x, pos.y))
        {
            if (bouton_signIn->getNomBouton() == "SignIn")
            {
                mode_login = false;
                ecran_login = false;
            }
        }
    }
    else
    {
        if (mode_login)
        {
            if (bouton_connection->isMouseOver(pos.x, pos.y))
            {
                // demande de login ici
                client->sendMessage(("l" + username->getText() + "\n" + password->getText()));
                if (client->getMessage() == "1")
                {
                    menu_ami->update();
                    username->empty();
                    password->empty();
                    ecran_login = true;
                    mode_login = false;
                    ecran_actuel = 1;
                }
                else
                {
                    interface->showPopup("Mot de passe incorrect ou \ncompte inexistant", true);
                }
            }
        }
        else
        {
            if (bouton_inscription->isMouseOver(pos.x, pos.y))
            {
                // demande d'inscription ici
                client->sendMessage(("r" + username->getText() + "\n" + password->getText()));
                if (client->getMessage() == "1")
                {
                    menu_ami->update();
                    ecran_actuel = 1;
                }
            }
        }
        if (bouton_retour->isMouseOver(pos.x, pos.y))
        {
            ecran_login = true;
            mode_login = false;
            username->empty();
            password->empty();
        }
    }
}

void LoginGUI::textEntre(sf::Event evenement, int &ecran_actuel)
{
    int charTyped = evenement.text.unicode;
    if (!ecran_login)
    {
        if (charTyped == ENTER_KEY && mode_login)
        {
            client->sendMessage(("l" + username->getText() + "\n" + password->getText()));
                if (client->getMessage() == "1")
                {
                    menu_ami->update();
                    username->empty();
                    password->empty();
                    ecran_login = true;
                    mode_login = false;
                    ecran_actuel = 1;
                }
                else
                {
                    interface->showPopup("Mot de passe incorrect ou \ncompte inexistant", true);
                }
        }
        else if (charTyped == ENTER_KEY && !mode_login)
        {
            client->sendMessage(("r" + username->getText() + "\n" + password->getText()));
                if (client->getMessage() == "1")
                {
                    menu_ami->update();
                    ecran_actuel = 1;
                }
        }
    }
    username->typedOn(evenement);
    password->typedOn(evenement);
}
