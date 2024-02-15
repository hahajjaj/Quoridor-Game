#include "Interface_GUI.hpp"

/** ecrans
 * 1 = menu principal
 * 2 = classement
 * 3 = parametres
 * 4 = login
 * 5 = menu ami
 * 6 = menu jeu
 * 7 = chat
 * 8 = reglement
 * 10 = plateau de jeu
 */

GUI::GUI(Client *client)
    : Interface(client)
{
}

GUI::~GUI()
{
}

void GUI::run()
{
    stop();
    status = true;
    mainThread = thread([&]() -> void
                        { handler(); });
}

bool GUI::initBoard()
{
    // plateau
    std::string res = client->getMessage(false);

    if (res[0] == '1')
    {
        int nombre_joueur = getNombreJoueur(res);
        if (!initPartie)
        {
            plateau = make_unique<BoardGui>(echelle, fenetre.get(), nombre_joueur, &font, &cadre_menu, ecran_actuel, client, this);
            initPartie = true;
        }
        hidePopup();
        plateau->updateBoard(res);
        ecran_actuel = 10;
        return true;
    }
    // else if (res == "8")
    // {
    //     menu_jeu->setModeBoolTournoi(false);
    // }
    return false;
}

void GUI::getResolution()
{
    std::ifstream fileSource("resolution.txt");
    if (!fileSource)
    {
        std::cerr << "Canot open "
                  << "resolution.txt" << std::endl;
        echelle = 1.2;
    }
    else
    {
        // Intermediate buffer
        std::string buffer;

        // By default, the >> operator reads word by workd (till whitespace)
        fileSource >> buffer;
        echelle = std::stof(buffer);
    }
}

void GUI::setResolution(float new_res)
{
    std::ofstream fileSink("resolution.txt");
    if (!fileSink)
    {
        std::cerr << "Canot open "
                  << "resolution" << std::endl;
    }
    else
    {
        fileSink << new_res;
    }
}

int GUI::getNombreJoueur(std::string etat)
{
    etat.erase(0, 1);
    std::string buff = "";
    const char *info = etat.c_str();
    int compteur = 0;
    // pour les pions
    for (int i = 0; i < 4; ++i)
    {
        bool var = false;
        for (int nbPion = *info++ - '0'; nbPion > 0; --nbPion)
        {
            info++;
            info++;
            var = true;
        }
        if (var)
        {
            compteur++;
        }
    }
    std::cout << "compteur :" << compteur << std::endl;
    return compteur;
}

void GUI::showPopup(std::string message, bool limitePopup)
{
    limitShowMessage = limitePopup;
    annonce.setString(message);
    annonce.setFont(font);
    annonce.setCharacterSize(echelle * 25);
    annonce.setPosition(sf::Vector2f(spriteCadrePopup.getPosition().x + (spriteCadrePopup.getGlobalBounds().width / 2.0) - (annonce.getGlobalBounds().width / 2.0), spriteCadrePopup.getPosition().y + (spriteCadrePopup.getGlobalBounds().height / 2.0) - (annonce.getGlobalBounds().height / 2.0) - (echelle * 50)));
    showMessage = true;
}

void GUI::hidePopup()
{
    showMessage = false;
}

void GUI::generateWindow()
{
    fenetre = make_unique<sf::RenderWindow>(sf::VideoMode(echelle * 1300, echelle * 1000), "Quoridor", sf::Style::Close);
    auto desktop = sf::VideoMode::getDesktopMode();
    fenetre->setPosition(sf::Vector2i((desktop.width / 2) - (fenetre->getSize().x / 2), (desktop.height / 2) - (fenetre->getSize().y / 2)));
    fenetre->setFramerateLimit(60);

    spriteBackgroundImage = sf::Sprite();
    spriteBackgroundImage.setTexture(backgroundImage);
    spriteBackgroundImage.setPosition(0, 0);
    spriteBackgroundImage.scale((echelle * 1300.0) / backgroundImage.getSize().x, (echelle * 1000.0) / backgroundImage.getSize().y);

    // popup
    spriteCadrePopup = sf::Sprite();
    spriteCadrePopup.setTexture(cadre_popup);
    spriteCadrePopup.scale((cadre_popup.getSize().x * (echelle * 3)) / cadre_popup.getSize().x, (cadre_popup.getSize().y * (echelle * 3)) / cadre_popup.getSize().y);
    spriteCadrePopup.setPosition(((echelle * width) / 2.0) - (spriteCadrePopup.getGlobalBounds().width / 2.0), ((echelle * height) / 2) - (spriteCadrePopup.getGlobalBounds().height / 2.0));

    // Image Cadre MenuGui

    // plateau->updateBoard("1401480010n9n0n0n0m0i0p0p0p0p10");

    // MenuGui
    if (!menus.empty())
    {
        for (auto &m : menus)
        {
            delete m;
        }
        menus.clear();
    }
    std::vector<std::string> nom_boutons_menu_principal{"Jouer", "Amis", "Regles du jeu", "Classement", "Parametre", "Deconnexion"};
    MenuGui *menu_principale = new MenuGui(echelle, "menu principal", 600, 900, ecran_actuel, nom_boutons_menu_principal, sf::Vector2f((1300.0 / 2) - 300, 50), &font, &cadre_menu);
    std::vector<int> ids{6, 5, 8, 2, 3};
    menu_principale->setIds(ids);
    menus.push_back(menu_principale);

    std::vector<std::string> nom_boutons_menu_classement{"Classement global", "Classement d'amis", "Classement hebdomadaire", "Retour"};
    MenuGui *menu_classement_old = new MenuGui(echelle, "menu_classement", 600, 800, ecran_actuel, nom_boutons_menu_classement, sf::Vector2f((1300.0 / 2) - 300, 100), &font, &cadre_menu);
    std::vector<int> ids3{10, 10, 10, 1};
    menu_classement_old->setIds(ids3);
    menus.push_back(menu_classement_old);

    std::vector<std::string> nom_boutons_menu_settings{"Petit", "Moyen", "Grand", "Retour"};
    MenuGui *menu_settings = new MenuGui(echelle, "menu paramètres", 600, 600, ecran_actuel, nom_boutons_menu_settings, sf::Vector2f((1300.0 / 2.0) - 300, 200), &font, &cadre_menu);
    menus.push_back(menu_settings);

    menu_chat = make_unique<MenuChat>(echelle, (echelle * 352) * 2.8, (echelle * 224) * 2.8, sf::Vector2f(((echelle * 1300) - ((echelle * 352) * 2.8)) / 2.0, ((echelle * 1000) - ((echelle * 224) * 2.8)) / 2.0), &font, client);
    menu_ami = make_unique<MenuAmi>(echelle, (echelle * 1100), (echelle * 900), &font, sf::Vector2f(((echelle * 1300.0) - (echelle * 1100.0)) / 2.0, ((echelle * 1000.0) - (echelle * 900.0)) / 2.0), client, menu_chat);
    menu_login = make_unique<LoginGUI>(echelle, (echelle * 1300), (echelle * 1000), &font, client, menu_ami, this);
    menu_jeu = make_unique<MenuJeu>((echelle * 1300), (echelle * 1000), &font, sf::Vector2f(((echelle * 1300.0) - (echelle * 1100.0)) / 2.0, ((echelle * 1000.0) - (echelle * 900.0)) / 2.0), client, *menu_principale, this);
    menu_classement = make_unique<MenuClassement>(echelle, (echelle * 1300), (echelle * 1000), &font, sf::Vector2f(((echelle * 1300.0) - (echelle * 1100.0)) / 2.0, ((echelle * 1000.0) - (echelle * 900.0)) / 2.0), client, this);
    menu_reglement = make_unique<MenuReglement>(echelle, (echelle * 1300), (echelle * 1000), &font, sf::Vector2f(((echelle * 1300.0) - (echelle * 1100.0)) / 2.0, ((echelle * 1000.0) - (echelle * 900.0)) / 2.0), client, this);
}

void GUI::handler()
{
    getResolution();

    // audio
    buffer_audio.loadFromFile("ressources/field_theme_2.wav");
    music_fond.setBuffer(buffer_audio);
    music_fond.setLoop(true);
    music_fond.play();

    cadre_popup.loadFromFile("ressources/cadre_popup.png");
    cadre_menu.loadFromFile("ressources/menu_assets/cadre_menu.png");

    // Font du Jeu
    font.loadFromFile("ressources/fontJeu2.ttf");

    // Image de fond
    backgroundImage.loadFromFile("ressources/map_menu.png");

    float timePopUp;
    generateWindow();

    while (status && fenetre->isOpen())
    {
        sf::Time frameTime = clock.restart();
        while (fenetre->pollEvent(evenement))
        {
            switch (evenement.type)
            {
            case sf::Event::Closed:
            {
                fenetre->close();
                client->stop();
                break;
            }
            case sf::Event::KeyPressed:
            {
                switch (evenement.key.code)
                {
                case sf::Keyboard::Delete:
                {
                    fenetre->close();
                    client->stop();
                    break;
                }
                default:
                    break;
                }
            }
            case sf::Event::MouseButtonReleased:
            {
                if (evenement.mouseButton.button == sf::Mouse::Left)
                {
                    if (ecran_actuel == 10)
                    {
                        plateau->updtate(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                    }
                    else if (ecran_actuel == 2)
                    {
                        menu_classement->mouseClick(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                    }
                    else if (ecran_actuel == 5)
                    {
                        menu_ami->mouseClick(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                    }
                    else if (ecran_actuel == 4)
                    {
                        menu_login->mouseClick(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                    }
                    else if (ecran_actuel == 6)
                    {
                        menu_jeu->mouseClick(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                    }
                    else if (ecran_actuel == 7)
                    {
                        menu_chat->mouseClick(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                    }
                    else if (ecran_actuel == 8)
                    {
                        menu_reglement->mouseClick(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                    }
                    else
                    {
                        if (ecran_actuel == 1)
                        {
                            for (auto &b : *menus[ecran_actuel - 1]->getBoutonsMenu())
                            {
                                if (b->contains(sf::Mouse::getPosition(*fenetre).x, sf::Mouse::getPosition(*fenetre).y))
                                {

                                    if (b->getNomCase() == "Jouer")
                                    {
                                        menu_jeu->update();
                                        menus[ecran_actuel - 1]->update(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                                    }
                                    else if (b->getNomCase() == "Deconnexion")
                                    {
                                        client->sendMessage("d");
                                        if (client->getMessage() == "1")
                                        {
                                            ecran_actuel = 4;
                                        }
                                    }
                                    else if (b->getNomCase() == "Amis")
                                    {
                                        menu_ami->update();
                                        menus[ecran_actuel - 1]->update(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                                    }
                                    else if (b->getNomCase() == "Classement")
                                    {
                                        menu_classement->update();
                                        menus[ecran_actuel - 1]->update(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                                    }
                                    else
                                    {
                                        menus[ecran_actuel - 1]->update(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                                    }
                                }
                            }
                        }
                        else if (ecran_actuel == 3)
                        {
                            for (auto &b : *menus[ecran_actuel - 1]->getBoutonsMenu())
                            {
                                if (b->contains(sf::Mouse::getPosition(*fenetre).x, sf::Mouse::getPosition(*fenetre).y))
                                {
                                    if (b->getNomCase() == "Petit")
                                    {
                                        echelle = 1;
                                        setResolution(echelle);
                                        updateResolution = true;
                                    }
                                    else if (b->getNomCase() == "Moyen")
                                    {
                                        echelle = 1.8;
                                        setResolution(echelle);
                                        updateResolution = true;
                                    }
                                    else if (b->getNomCase() == "Grand")
                                    {
                                        echelle = 2.4;
                                        setResolution(echelle);
                                        updateResolution = true;
                                    }
                                    else if (b->getNomCase() == "Retour")
                                    {
                                        ecran_actuel = 1;
                                    }
                                }
                            }
                        }
                        else
                        {
                            menus[ecran_actuel - 1]->update(sf::Mouse::getPosition(*fenetre), ecran_actuel);
                        }
                    }
                }
                break;
            }
            case sf::Event::TextEntered:
            {
                if (ecran_actuel == 5)
                {
                    menu_ami->textEntre(evenement);
                }
                if (ecran_actuel == 4)
                {
                    menu_login->textEntre(evenement, ecran_actuel);
                }
                if (ecran_actuel == 7)
                {
                    menu_chat->textEntre(evenement);
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (ecran_actuel == 10)
                {
                    plateau->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
                else if (ecran_actuel == 2)
                {
                    menu_classement->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
                else if (ecran_actuel == 5)
                {
                    menu_ami->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
                else if (ecran_actuel == 4)
                {
                    menu_login->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
                else if (ecran_actuel == 6)
                {
                    menu_jeu->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
                else if (ecran_actuel == 7)
                {
                    menu_chat->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
                else if (ecran_actuel == 8)
                {
                    menu_reglement->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
                else
                {
                    menus[ecran_actuel - 1]->mouseMove(sf::Mouse::getPosition(*fenetre));
                }
            }
            default:
                break;
            }
        }
        fenetre->clear();
        // animation.update(clock.getElapsedTime().asSeconds());
        if (ecran_actuel == 10)
        {
            plateau->draw(frameTime);
        }
        else
        {
            fenetre->draw(spriteBackgroundImage);
            if (ecran_actuel == 2)
            {
                menu_classement->drawTo(*fenetre);
            }
            else if (ecran_actuel == 4)
            {
                menu_login->drawTo(*fenetre);
            }
            else if (ecran_actuel == 5)
            {
                menu_ami->drawTo(*fenetre, frameTime);
            }
            else if (ecran_actuel == 6)
            {
                menu_jeu->drawTo(*fenetre, frameTime);
            }
            else if (ecran_actuel == 7)
            {
                menu_chat->drawTo(*fenetre);
            }
            else if (ecran_actuel == 8)
            {
                menu_reglement->drawTo(*fenetre);
            }
            else
            {
                menus[ecran_actuel - 1]->drawTo(*fenetre);
            }
        }
        if (showMessage)
        {
            fenetre->draw(spriteCadrePopup);
            fenetre->draw(annonce);
            if (limitShowMessage)
            {
                timePopUp += frameTime.asSeconds();
                if (timePopUp > 2.0f)
                {
                    showMessage = false;
                    limitShowMessage = false;
                    timePopUp = 0;
                }
            }
        }
        fenetre->display();
        if (updateResolution)
        {
            updateResolution = false;
            generateWindow();
        }
    }
    if (fenetre->isOpen())
        fenetre->close();
    status = false;
}
