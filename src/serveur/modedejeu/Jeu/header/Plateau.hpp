#ifndef __PLATEAU_HPP
# define __PLATEAU_HPP

#include <iostream>
#include <vector>
#include "Position.hpp"
#include "elements/Pion.hpp"
#include "Case.hpp"
#include "elements/Mur.hpp"

class Serveur;

class Plateau
{
protected:
    std::vector<Mur*> murs;
    std::vector<std::vector<Case *>> plateau;
    std::vector<Pion *> pions;
    int taille;
    int nbrJoueur;
    Serveur *serveur;
public:
    Plateau(int p_taille, int nombreJoueur, bool isItem);
    Plateau(int p_taille, int nombreJoueur, Serveur *serveur);
    Plateau(int p_taille, int nombreJoueur);
    Plateau(const Plateau &other)=default;
    virtual ~Plateau();
    virtual void initializePLateau();
    virtual void initializePion();
    virtual void afficherCase(int y, int x);

    virtual void affichagePlateau();
    bool respecteDelimitation(int y, int x);
    virtual bool poserMurs(Pion *p, Point<> posistion1, Point<> position2);
    virtual bool isMur(Point<> pos_init, Point<> pos_fin);
    virtual std::vector<Mur*> getMurs();
    std::vector<std::vector<Case *>> getPlateau(){return plateau;}
    std::vector<Pion *> getPions(){return pions;};
    int getTaille(){return taille;};    
    int String_to_plateau(std::string etat_plateau, int nbrJoueurs);
    int getNbrJoueur(){return nbrJoueur;}
    virtual void supprimerMurs();
    virtual void remettreVisiteAZero();

};



#endif