#include "../header/PlateauItem.hpp"


PlateauItem::PlateauItem(int p_taille, int nombreJoueur): Plateau(p_taille, nombreJoueur, true)
{
    pions.clear();
    plateau.clear();
    initializePLateau();
    initializePion();
    initialiseItem();
}

PlateauItem::~PlateauItem(){

}
void PlateauItem::initializePLateau()
{
    for (int y = 0; y < getTaille(); y++)
    {
        plateau.push_back({});
        for (int x = 0; x < getTaille(); x++)
        {
            CaseItem *c = new CaseItem(x, y);
            plateau[y].push_back(c);
        }
    }
}

void PlateauItem::initializePion()
{
    PionItem *p = new PionItem(4, 0, 1);
    PionItem *p2 = new PionItem(4, 8, 2);
    pions.push_back(p);
    pions.push_back(p2);
    plateau[0][4]->setPionCase(p);
    plateau[8][4]->setPionCase(p2);
    if (getNbrJoueur() == 4)
    {
        PionItem *p3 = new PionItem(0, 4, 3);
        PionItem *p4 = new PionItem(8, 4, 4);
        pions.push_back(p3);
        pions.push_back(p4);
        plateau[4][0]->setPionCase(p3);
        plateau[4][8]->setPionCase(p4);
    }
}


void PlateauItem::initialiseItem(){
    Item *i = new Item("traverser");
    i->setPosition(new Point<>{0,4});
    Item *j = new Item("supprimer");
    j->setPosition(new Point<>{8,4});
    items.push_back(i);
    items.push_back(j);
    dynamic_cast<CaseItem*>(plateau[4][0])->setItem(i);
    dynamic_cast<CaseItem*>(plateau[4][8])->setItem(j);
}

void PlateauItem::afficherCase(int y, int x){
    if(dynamic_cast<CaseItem*>(plateau[y][x])->isItem()){
        if(dynamic_cast<CaseItem*>(plateau[y][x])->getItem()->getitem() == "traverser"){
            std::cout << "t";
        }
        else if(dynamic_cast<CaseItem*>(plateau[y][x])->getItem()->getitem() == "supprimer"){
            std::cout << "s";
        }
    }
    else
        Plateau::afficherCase(y, x);
}

void PlateauItem::affichagePlateau()
{
    Plateau::affichagePlateau();
    std::cout << "plateau item" << std::endl;
}



bool PlateauItem::supprimerMurItem(Point<> pos1, Point<> pos2)
{
    if (isMur(pos1, pos2))
    {
        for (auto &m : murs)
        {
            
            if ((pos1.x == m->getPositionMur().first.x && pos1.y == m->getPositionMur().first.y) || (pos1.x == m->getPositionMur().second.x && pos1.y == m->getPositionMur().second.y))
            {
                
                if ((pos2.x == m->getPositionMur().first.x && pos2.y == m->getPositionMur().first.y) || (pos2.x == m->getPositionMur().second.x && pos2.y == m->getPositionMur().second.y))
                {
                    murs.erase(find(murs.begin(), murs.end(), m));
                    return true;
                }
            }
        }
    }
    return false;
}

void PlateauItem::supprimerItemPlateau(Item* i){
    items.erase(find(items.begin(), items.end(), i));
}