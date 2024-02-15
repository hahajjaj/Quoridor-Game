#ifndef __JEUITEM_HPP
#define __JEUITEM_HPP

#include "../../Jeu/header/Jeu.hpp"
#include "Item.hpp"
#include "PlateauItem.hpp"

class Serveur;
class JeuItem : public Jeu
{
public:
    JeuItem(int nombreJoueur, Serveur *serveur);
    JeuItem(int nombreJoueur, Partie *partie, Plateau *plateau);
    JeuItem(int nombreJoueur, Partie *partie, string plateau);
    JeuItem(int nombreJoueur);
    ~JeuItem();

    void run();
    std::string parseMessage();
    Item getItem();
    bool jouerCoup(std::string coup);
    void deplacerPion(Point<> initial, Point<> final);
    bool traverserMur();
    bool verifMouvement(Point<> position_init, Point<> position_final);
    bool poserMur(Point<> position_init, Point<> position_final);
};

#endif
