#ifndef __PLATEAUITEM_HPP
#define __PLATEAUITEM_HPP

#include "../../Jeu/header/Plateau.hpp"
#include "PionItem.hpp"
#include <vector>
#include "caseItem.hpp"


class PlateauItem: public Plateau
{
private:
    vector<Item*> items;
public:
    PlateauItem(int p_taille, int nombreJoueur, Serveur *serveur);
    PlateauItem(int p_taille, int nombreJoueur);
    ~PlateauItem();
    void initialiseItem();
    void initializePion() override;
    void initializePLateau() override;
    void afficherCase(int y, int x) override;

    void affichagePlateau() override;
    void supprimerItemPlateau(Item* i);
    bool supprimerMurItem(Point<> pos1, Point<> pos2);
    vector<Item*> getVectorItems(){return items;}
};

#endif