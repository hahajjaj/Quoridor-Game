#ifndef __PARTIE_HPP
#define __PARTIE_HPP
#include "../common/common.hpp"

class User;
class Jeu;
class Serveur;
class JeuItem;


class Partie
{
    Serveur *srv;
    int nbJoueursConnected = 0;
    int nbJoueur;
    int turn_to_play = 0;
    bool readyBool = false;
public:
    bool FromSauvegarde = false;
    string etat_plateau = "";
    int joueur_gagnant = 0;
    vector<User *> users;
    unique_ptr<Jeu> jeu;
    string ModeJeu = "Normal";

    bool isFinished();
    Partie(User *user, Serveur *s, int nbJoueur = 2);
    ~Partie(); 
    void lancePartie(string plateau);
    void lancePartie();
    void setReady(){readyBool = true;}
    bool isReady() const { return readyBool; }
    void setNbJoueur(int j);
    int nbJoueurPret();
    bool joueurPret();
    void addUser(User *user);
    void play(string plateau);
    void play();
    void sendAll(string message);
    void sendAllBoard(string message);
    bool jouerCoup(char *message);
    Jeu *getJeu();
    User *getUser();
};

#endif