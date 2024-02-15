#ifndef __JEU_HPP
# define __JEU_HPP

# include <string>
# include <iostream>
# include <queue>

# include "ModeDeFonctionement.hpp"
# include "elements/Pion.hpp"
# include "Plateau.hpp"
# include "../../../Partie.hpp"

class Serveur;
class Jeu
{
protected:
    int nbJoueur;
    unique_ptr<Plateau> plateau;
    string plt;
    Partie *partie;
    int gagnant = 1;

    void saveGame();
    void loadGame();  //Servira pour la partie graphique
    void createGame(Serveur *serveur);  // Crée le plateau de jeu ainsi que les pions
    
    virtual bool ExistChemin(Point<> p);
public:
    int joueur_actuel = 1;
    Jeu(int nombreJoueur, Serveur *serveur);
    Jeu(int nombreJoueur, Partie *partie,  Plateau *p );
    Jeu(int nombreJoueur);

    Point<> coupToPoint(std::string coup);
    virtual std::string parseMessage();
    Jeu(int nombreJoueur, Partie *partie, string plateau);
    void sendMessage();
    void modifieDataBase();
    virtual ~Jeu();
    virtual std::string parsePlateauString();
    virtual bool jouerCoup(std::string coup);
    virtual bool FinPartie();
    virtual bool getMessage(char *message);
    int getNbrJoueur(){return nbJoueur;}
    virtual int getJoueurActuel(){return joueur_actuel;}
    virtual void setJoueurActuel(int new_joueur);
    virtual void deplacerPion(Point<> initial, Point<> final);
    virtual bool poserMur(Point<> position_init, Point<> position_final);
    virtual bool verifMouvement(Point<> position_init, Point<> position_final);
    virtual bool isNeighbour(Point<> initial, Point<> final);
    virtual void run();
    // Plateau* getPlateau();
};

#endif
