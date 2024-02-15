#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "../common/common.hpp"
#include "Client.hpp"

const string MENU = "Jouer\nAmis\nRègles du jeu\nClassement\nDéconnexion";
const string LOGIN = "Login\nSign in"; // TODO ajout exit
const string AMIS = "Gérer la liste d'amis\nVoir les classements\nRetour";
const string MENU_MESSAGERIE = "Envoyer une demande d'ami\nAccepter une demande d'ami\nRefuser une demande d'ami\nSupprimer un ami\nMessagerie\nAfficher la liste d'amis\nAfficher la liste des demande d'amis\nRetour";
const string MENU_CLASSEMENT = "Classment global\nClassement d'amis\nClassement hebdomadaire\nRetour";
const string MENU_JOUE = "Créer une partie\nRejoindre une partie rapide\nReprendre une partie sauvegardée\nRetour";
const string MENU_CONFIGPARTIE = "Lancer la partie\nMode de jeu\nNombre de joueur\nInviter des joueurs\nRetour";
const string REGLE_JEU = "PRESENTATION\n\
* un plateau de jeu.\n\
* 2 zones de stockage.\n\
* 20 barrières et 4 pions.\n\
BUT DU JEU\n\
Atteindre le premier la ligne opposée à sa ligne de départ\n\
REGLE POUR 2 JOUEURS\n\
En début de partie, les barrières sont remises dans leur zone de stockage (10 par joueur).\n\
Chaque joueur pose son pion au centre de sa ligne de départ.\n\
Un tirage au sort détermine qui commence.\n\
Déroulement d\'une partie\n\
A tour de rôle, chaque joueur choisit de déplacer son pion ou de poser une de ses barrières.\n\
Lorsqu\'il n\'a plus de barrières, un joueur est obligé de déplacer son pion. Déplacement des pions:\n\
les pions se déplacent d\'une case, horizontalement ou verticalement, en avant ou en arrière :\n\
les barrières doivent être contournées.\n\
Pose des barrières:\n\
une barrière doit être posée exactement entre 2 blocs de 2 cases.\n\
La pose des barrières a pour but de se créer son propre chemin ou de ralentir l\'adversaire, mais il est interdit de lui fermer totalement l\'accès à sa ligne de but:\n\
il faut toujours lui laisser une solution.\n\
Face à face:\n\
Quand les 2 pions se retrouvent en vis-à-vis sur 2 cases voisines non séparées par une barrière,\n\
le joueur dont c\'est le tour peut sauter son adversaire et se placer derrière lui.\n\
Si une barrière est située derrière le pion sauté, le joueur peut choisir de bifurquer à droite ou à gauche du pion sauté.\n\
FIN DE LA PARTIE\n\
Le premier joueur qui atteint une des 9 cases de la ligne opposée à sa ligne de départ gagne la partie.\n\
DUREE D\'UNE PARTIE\n\
De 10 à 20 minutes.\n\
REGLE POUR 4 JOUEURS\n\
En début de partie, les 4 pions sont disposés au centre de chacun des 4 côtes du plateau et chaque joueur dispose de 5 barrières.\n\
Les règles sont strictement identiques, mais on ne peut sauter plus d\'un pion à la fois";

class Interface
{
protected:
    Client *client;
    bool status = false;
    mutex Mutex;
    thread mainThread;

    virtual void handler() = 0;

public:
    Interface(Client *client) : client{client} {}
    virtual ~Interface() {}

    virtual void run()
    {
        stop();
        status = true;
        mainThread = thread([&]() -> void { handler(); });
    }
    virtual bool isRunning() const { return status; }
    virtual void stop()
    {
        // if (status)
            // pthread_cancel(mainThread.native_handle());
        status = false;
        if (mainThread.joinable())
            mainThread.join();
    }
};

#endif // __INTERFACE_H__