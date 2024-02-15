
#ifndef __SERVEUR_HPP
#define __SERVEUR_HPP

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <algorithm>

#include <netinet/in.h>
#include <sys/socket.h>
#include "../common/common.hpp"
#include "basededonnee/BaseDeDonnee.hpp"
#include "Partie.hpp"
#include "Tournoi.hpp"
#include "User.hpp"

const string MENU = "Jouer\nAmis\nRègles du jeu\nClassement\nDéconnexion";
const string LOGIN = "Login\nSign in";
const string AMIS = "Gérer la liste d'amis\nVoir les classements\nRetour";
const string MENU_MESSAGERIE = "Envoyer une demande d'ami\nAccepter une demande d'ami\nRefuser une demande d'ami\nSupprimer un ami\nMessagerie\nAfficher la liste d'amis\nAfficher la list des demande d'amis\nRetour au menu précédent";
const string MENU_CLASSEMENT = "Classment global\nClassement d'amis\nClassement hebdomadaire\nRetour";
const string MENU_JOUE = "Créer une partie\nRejoindre une partie rapide\nReprendre une partie sauvegardée\nRetour";
const string MENU_CONFIGPARTIE = "Mode de jeu\nNombre de joueur\nInviter des joueurs\nLancer une partie en local\nLancer la partie.";
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
// En tournoi, il est possible d\'allouer à chaque joueur un temps limité.\n

class Jeu;
class JeuItem;
class Tournoi;

/////////////////// SERVEUR ///////////////////////////////////////////////

class Serveur : public ServeurClient
{
private:
    /* data */
    size_t addrlen;
    int serveur_fd;
    vector<User *> users;
    string chatlog = "";
    shared_ptr<BaseDeDonnee> db;

    int set_fd_sets(fd_set &readfds);

public:
    Serveur();
    ~Serveur();
    void sendTo(User *u, const string &m);
    void parseJoue(string message, User *u);
    void parseMovePartie(string message, User *u);
    void parseAmis(string message, User *u);
    void parseClass(string message, User *u);
    void parseChat(string message, User *u);
    void parseJoinPartie(string message, User *u);
    void parseLogin(string message, User *u);

    void gameDeletePartie(string message, User *u);
    void gameForfaitPartie(string message, User *u);
    void gameLancePartie(string message, User *u);
    void savePartie(string message, User *u);
    void gameReprendrePartie(string message, User *u);
    void gameSendInvite(string message, User *u);

    User *findUser(const char *username);
    string getConnectedFriends(User *u, bool offline = true);
    void parseGui(string message, User *u);
    string reprendrePartieSauvegarde(string message, User *u);
    void newClient();
    void disconnectCLient(User *u);
    bool messageReceive(fd_set &readfds);
    void run(int port);
    int recvMessage(int sock, string &dest, bool block = true);
    void removeAllInvitGame(User *u);
};

// #include "modedejeu/Jeu/header/Jeu.hpp"
#include "modedejeu/JeuItem/header/JeuItem.hpp"

#endif
