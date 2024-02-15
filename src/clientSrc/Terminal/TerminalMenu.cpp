#include "TerminalMenu.hpp"
#include "TerminalMode.hpp"


MenuLogin::MenuLogin(Terminal &terminal)
    : Terminal::Menu(terminal)
{
}

MenuLogin::~MenuLogin()
{
}

void MenuLogin::run()
{
    mode = make_unique<MMenu>(LOGIN);
    while (terminal.isRunning())
    {
        string pseudo;
        string choice = getInput();
        switch (choice[0])
        {
        case '1': // login
        case '2': // sign in
        {
            mode = make_unique<Text>("Pseudo:");
            pseudo = getInput();
            mode = make_unique<Password>("Password:");
            string password = getInput();
            terminal.getClient()->sendMessage(format("%c%s\n%s", choice[0] == '1' ? 'l' : 'r', pseudo.c_str(), password.c_str()));
            break;
        }
        default:
            continue;
        }
        string response = terminal.getClient()->getMessage();
        if (response[0] == '1') // succes
        {
            terminal.setCurrMenu(new MenuPrincipal(terminal));
            terminal.setPseudo(pseudo);
            return;
        }
        else // error
        {
            mode = make_unique<MMenu>("Error: " + response.substr(1) + "\x1f\n" + LOGIN);
            continue;
        }
        mode = make_unique<MMenu>(LOGIN);
    }
}

MenuPrincipal::MenuPrincipal(Terminal &terminal)
    : Terminal::Menu(terminal)
{
}

MenuPrincipal::~MenuPrincipal()
{
}

void MenuPrincipal::run()
{
    mode = make_unique<MMenu>(MENU);
    while (terminal.isRunning())
    {
        string choice = getInput();
        switch (choice[0])
        {
        case '1': // Joue
            terminal.setCurrMenu(new MenuJoue(terminal));
            return;
        case '2': // Amis
            terminal.setCurrMenu(new MenuAmis(terminal));
            return;
        case '3': // Règle du jeu
            mode = make_unique<Text>(REGLE_JEU);
            getInput();
            break;
        case '4': // Classement
        {
            mode->set(MENU_CLASSEMENT);
            bool status = true;
            while (status && terminal.isRunning())
            {
                choice = getInput();
                switch (choice[0])
                {
                case '1': // Classement global
                case '2': // Classement amis
                case '3': // Classement hebdo
                {
                    terminal.getClient()->sendMessage("c" + (string)(choice[0] == '1' ? "g" : (choice[0] == '2' ? "f" : "h")));
                    string response = terminal.getClient()->getMessage();
                    if (response[0] == '1') // succes
                    {
                        mode = make_unique<Classement>(response.substr(1));
                        getInput();
                    }
                    else // error
                    {
                        mode->set("Error: " + response.substr(1) + "\x1f\n" + MENU_CLASSEMENT);
                        continue;
                    }
                    break;
                }
                case '4': // retour
                default:
                    status = false;
                    continue;
                }
                mode = make_unique<MMenu>(MENU_CLASSEMENT);
            }
            break;
        }
        case '5': // Déconnexion
        {
            terminal.getClient()->sendMessage("d");
            string response = terminal.getClient()->getMessage();
            if (response[0] == '0')
            {
                mode->set("Error: " + response.substr(1) + "\x1f\n" + MENU);
                continue;
            }
            terminal.resetPseudo();
            terminal.setCurrMenu(new MenuLogin(terminal));
            return;
        }
        default:
            break;
        }
        mode = make_unique<MMenu>(MENU);
    }
}

MenuAmis::MenuAmis(Terminal &terminal)
    : Terminal::Menu(terminal)
{
}

MenuAmis::~MenuAmis()
{
}

void MenuAmis::run()
{
    mode = make_unique<MMenu>(MENU_MESSAGERIE);
    while (terminal.isRunning())
    {
        string choice = getInput();
        string response = "1";
        string message = choice[0] == '1' ? "fi" : "";
        if (choice[0] == '2' || choice[0] == '3' || choice[0] == '7')
        {
            terminal.getClient()->sendMessage("fgi");
            response = terminal.getClient()->getMessage();
            message = choice[0] == '2' ? "fa" : (choice[0] == '3' ? "fr" : "");
        }
        else if (choice[0] == '4')
        {
            terminal.getClient()->sendMessage("fgf");
            response = terminal.getClient()->getMessage();
            message = choice[0] == '4' ? "fd" : "";
        }
        else if (choice[0] == '5' || choice[0] == '6')
        {
            terminal.getClient()->sendMessage("fgco");
            response = terminal.getClient()->getMessage();
        }
        if (response[0] == '0')
        {
            mode->set("Error: " + response.substr(1) + "\x1f\n" + MENU_MESSAGERIE);
            continue;
        }
        switch (choice[0])
        {
        case '1': // Envoyer demande d'amis
        case '2': // Accepter demande d'amis
        case '3': // Refuser demande d'amis
        case '4': // Supprimer amis
        {
            mode = make_unique<Text>("Entrez un pseudo:" + (response.size() > 1 ? "\n\n" + response.substr(1) : ""));
            string pseudo = getInput();
            if (pseudo.empty()) // ESC
                break;
            terminal.getClient()->sendMessage(message + pseudo);
            response = terminal.getClient()->getMessage();
            if (response[0] == '1')
                mode = make_unique<MMenu>("Done\x1f\n" + MENU_MESSAGERIE);
            else
                mode = make_unique<MMenu>("Error: " + response.substr(1) + "\x1f\n" + MENU_MESSAGERIE);
            continue;
        }
        case '5': // Messagerie
        {
            mode = make_unique<Text>("Entrez un pseudo:\n\n" + response.substr(1));
            string pseudo = getInput();
            if (pseudo.empty()) // ESC
                break;
            terminal.getClient()->sendMessage("mg" + pseudo);
            mode = make_unique<Text>(response.substr(1), true);
            bool status = true;
            thread sendThread{[&]() -> void
                              {
                                  while (status && terminal.isRunning())
                                  {
                                      message = getInput();
                                      if (message.empty())
                                      {
                                          status = false;
                                          terminal.getClient()->sendMessage("mg");
                                          break;
                                      }
                                      terminal.getClient()->sendMessage("ms" + message);
                                  }
                              }};
            thread receiveThread{[&]() -> void
                                 {
                                     while (status && terminal.isRunning())
                                     {
                                         response = terminal.getClient()->getMessage();
                                         if (response[0] == '0')
                                         {
                                             status = false;
                                             mode = make_unique<MMenu>("Error: " + response.substr(1) + "\x1f\n" + MENU_MESSAGERIE);
                                             break;
                                         }
                                         mode->set(response.substr(1) + "(Appuyez sur ESC pour quitter)");
                                         mode->show(terminal.getWriteWin());
                                     }
                                 }};
            sendThread.join();
            receiveThread.join();
            
            // terminal.getClient()->getMessage();
            break;
        }
        case '6': // Liste amis
        case '7': // Liste demande d'amis
            mode = make_unique<Text>(response.substr(1) + "\n(Appuyez sur une touche pour quitter)");
            getInput(true);
            break;
        case '8': // Retour
        default:
            terminal.setCurrMenu(new MenuPrincipal(terminal));
            return;
        }
        mode = make_unique<MMenu>(MENU_MESSAGERIE);
    }
}

MenuJoue::MenuJoue(Terminal &terminal)
    : Terminal::Menu(terminal)
{
}

MenuJoue::~MenuJoue()
{
}

void MenuJoue::run()
{
    mode = make_unique<MMenu>(MENU_JOUE);
    while (terminal.isRunning())
    {
        string choice = getInput();
        switch (choice[0])
        {
        case '1': // Cree un partie
        {
            string modeJeu{"Normal"}, nbJoueur{"2"};
            vector<string> invites;
            int nbInvite = 0;
            bool status = true;
            mode->set(MENU_CONFIGPARTIE + "\n\x1f\n" + vecToStrInvite(invites) + "\x1f");
            while (status && terminal.isRunning())
            {
                choice = getInput();
                switch (choice[0])
                {
                case '1': // Lance partie
                {
                    if ((modeJeu == "Tournoi" && nbInvite == 0) || (modeJeu != "Tournoi" && nbInvite != stoi(nbJoueur) - 1))
                    {
                        mode->set("Error: Vous n'avez pas invité assez ou trop d'amis\x1f\n" + MENU_CONFIGPARTIE + "\n\x1f\n" + vecToStrInvite(invites) + "\x1f");
                        continue;
                    }
                    string response;
                    for (auto &pseudo : invites)
                    {
                        terminal.getClient()->sendMessage("gs" + pseudo);
                        response = terminal.getClient()->getMessage();
                        if (response[0] == '0')
                        {
                            break;
                        }
                    }
                    if (response[0] == '0')
                    {
                        mode->set("Error: " + response.substr(1) + "\x1f\n" + MENU_CONFIGPARTIE + "\n\x1f\n" + vecToStrInvite(invites) + "\x1f");
                        break;
                    }
                    terminal.getClient()->sendMessage("gl" + modeJeu + "\n" + nbJoueur);
                    response = terminal.getClient()->getMessage();
                    if (response[0] == '0')
                    {
                        mode->set("Error: " + response.substr(1) + "\x1f\n" + MENU_CONFIGPARTIE + "\n\x1f\n" + vecToStrInvite(invites) + "\x1f");
                        continue;
                    }
                    terminal.setCurrMenu(new Game(terminal));
                    return;
                }
                case '2': // Change mode
                    mode = make_unique<MMenu>("Mode de Jeu (" + modeJeu + "):\x1f\nNormal\nItem\nTournoi\nRetour");
                    choice = getInput();
                    switch (choice[0])
                    {
                    case '1':
                        modeJeu = "Normal";
                        break;
                    case '2':
                        modeJeu = "Item";
                        break;
                    case '3':
                        modeJeu = "Tournoi";
                        break;
                    case '4':
                    default:
                        break;
                    }
                    break;
                case '3': // Change nb player
                    mode = make_unique<MMenu>("Nombre de Joueur (" + nbJoueur + "):\x1f\n2\n4\nRetour");
                    choice = getInput();
                    switch (choice[0])
                    {
                    case '1':
                        nbJoueur = "2";
                        break;
                    case '2':
                        nbJoueur = "4";
                    case '3':
                    default:
                        break;
                    }
                    break;
                case '4': // Invite player
                {
                    terminal.getClient()->sendMessage("fgc");
                    string response = terminal.getClient()->getMessage();
                    if (response[0] == '0')
                    {
                        mode->set("Error: " + response.substr(1) + "\x1f\n" + MENU_CONFIGPARTIE + "\n\x1f\n" + vecToStrInvite(invites) + "\x1f");
                        continue;
                    }
                    mode = make_unique<Text>("Entrez un pseudo à ajouter ou supprimer:\n\n" + response.substr(1));
                    string pseudo = getInput();
                    
                    if (pseudo.empty())
                        break;
                    
                    bool found = false;
                    for (auto it = invites.begin(); it != invites.end(); ++it)
                    {
                        if (*it == pseudo)
                        {
                            invites.erase(it);
                            found = true;
                            --nbInvite;
                            break;
                        }
                    }
                    if (found)
                        break;
                    terminal.getClient()->sendMessage("fv" + pseudo);
                    response = terminal.getClient()->getMessage();
                    if (response[0] != '1')
                    {
                        mode = make_unique<MMenu>("Error: " + response.substr(1) + "\x1f\n" + MENU_CONFIGPARTIE + "\n\x1f\n" + vecToStrInvite(invites) + "\x1f");
                        continue;
                    }
                    ++nbInvite;
                    invites.push_back(pseudo);
                    break;
                }
                case '5': // Retour
                default:
                    status = false;
                    break;
                }
                mode = make_unique<MMenu>(MENU_CONFIGPARTIE + "\n\x1f\n" + vecToStrInvite(invites) + "\x1f");
            }
            break;
        }
        case '2': // Rejoindre partie
        {
            terminal.getClient()->sendMessage("ggi");
            string response = terminal.getClient()->getMessage();
            if (response[0] == '0')
            {
                mode = make_unique<MMenu>("Error: " + response.substr(1) + "\x1f\n" + MENU_JOUE);
                continue;
            }
            mode = make_unique<Text>("Entrez un pseudo:\n\n" + response.substr(1) + "\n(Appuyez sur ESCAPE pour revenir en arrière)");
            string pseudo = getInput();
            if (pseudo.empty()) // ESC
                break;
            terminal.getClient()->sendMessage("gj" + pseudo);
            response = terminal.getClient()->getMessage();
            if (response[0] == '0')
            {
                mode = make_unique<MMenu>("Error: " + response.substr(1) + "\x1f\n" + MENU_JOUE);
                continue;
            }
            terminal.setCurrMenu(new Game(terminal));
            return;
        }
        case '3': // Reprendre partie
        {
            terminal.getClient()->sendMessage("ggs");
            string response = terminal.getClient()->getMessage();
            if (response[0] == '0')
            {
                mode = make_unique<MMenu>("Error: " + response.substr(1) + "\x1f\n" + MENU_JOUE);
                continue;
            }
            mode = make_unique<MMenu>("Partie sauvergardée(s):\x1f\n(Une invitation sera envoyé à/aux adversaire(s))\x1f\nRetour\n" + response.substr(1));
            choice = getInput();
            if (choice[0] == '1') // Retour
                break;
            int maxCount = stoi(choice);
            response.erase(response.begin());   // remove "1<response>"
                                                //!     ~~~^~~~
            for (int count = 2; count < maxCount; ++count)
            {
                auto sep = response.find('\n');
                response = response.substr(sep != string::npos ? sep : 0);
            }
                
            response = response.substr(0, response.find('\n'));
            terminal.getClient()->sendMessage("gr" + response);
            response = terminal.getClient()->getMessage();
            if (response[0] == '0')
            {
                mode->set("Error: " + response.substr(1) + "\x1f\n" + MENU_JOUE);
                continue;
            }
            terminal.setCurrMenu(new Game(terminal));
            return;
        }
        case '4': // Retour
        default:
            terminal.setCurrMenu(new MenuPrincipal(terminal));
            return;
        }
        mode = make_unique<MMenu>(MENU_JOUE);
    }
}

Game::Game(Terminal &terminal)
    : Terminal::Menu(terminal)
{
}

Game::~Game()
{
}

void Game::run()
{
    string attente = "En attente des autres joueurs";
    while (terminal.isRunning())
    {
        mode = make_unique<Text>(attente);
        mode->show(terminal.getWriteWin());
        string response = terminal.getClient()->getMessage();
        if (response[0] != '1')
        {
            mode->set("Error: " + response.substr(1));
            getInput(true);
            terminal.setCurrMenu(new MenuPrincipal(terminal));
            return;
        }
        mode = make_unique<Board>(response.substr(1));
        bool pause = false, status = true;
        thread sendThread{[&]() -> void
                            {
                                while (status && terminal.isRunning())
                                {
                                    string action = getInput();
                                    if (action == "pause" || action == "forfait")
                                    {
                                        pause = true;
                                        terminal.getClient()->sendMessage("g" + action);
                                    } 
                                    else if (!action.empty())
                                        terminal.getClient()->sendMessage("gm" + action);
                                }
                            }};
        thread receiveThread{[&]() -> void
                            {
                                while (status && terminal.isRunning())
                                {
                                    response = terminal.getClient()->getMessage();
                                    if (pause && response[0] == '0')
                                    {
                                        mvwprintw(terminal.getWriteWin(), 0, 0, ("Error: " + response.substr(1)).c_str());
                                        pause = false;
                                    }
                                    else if (response[0] == '1')
                                    {
                                        mode->set(response.substr(1));
                                        mode->show(terminal.getWriteWin());
                                    }
                                    else // game over or error
                                    {
                                        status = false;
                                        pthread_cancel(sendThread.native_handle());
                                    }
                                }
                            }};
        sendThread.join();
        receiveThread.join();
        if (response[0] == '0')
            mode = make_unique<Text>("Error:\n" + response.substr(1) + "\n(Appuyez sur ESCAPE pour revenir au menu principal)");
        else if (response[0] == '3')
        {
            attente = response.substr(1);
            continue;
        }
        else
            mode = make_unique<Text>(response.substr(1) + "\n(Appuyez sur ESCAPE pour revenir au menu principal)");
        getInput(true);
        terminal.setCurrMenu(new MenuPrincipal(terminal));
        return;
    }
    
}