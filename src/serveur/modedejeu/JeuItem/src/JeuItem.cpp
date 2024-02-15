#include "../header/JeuItem.hpp"
#include "../../../Serveur.hpp"

JeuItem::JeuItem(int nombreJoueur, Serveur *serveur) : Jeu(nombreJoueur, serveur){
}

JeuItem::JeuItem(int nombreJoueur) : Jeu(nombreJoueur){
    plateau = unique_ptr<PlateauItem>(new PlateauItem(9, 2));
}

JeuItem::JeuItem(int nombreJoueur, Partie *partie,  Plateau *p): Jeu(nombreJoueur, partie, p){
    // plateau = unique_ptr<PlateauItem>(new PlateauItem(9, nbJoueur));
}

JeuItem::JeuItem(int nombreJoueur, Partie *partie, string plt): Jeu(nombreJoueur, partie, plt){
    // a partir de plt creer plateau

    plateau = unique_ptr<PlateauItem>(new PlateauItem(9,nbJoueur));
    joueur_actuel = plateau->String_to_plateau(plt,nombreJoueur);
}

JeuItem::~JeuItem()
{
}

// Boucle générale du jeu
void JeuItem::run()
{
    std::system("clear");
    bool gagnant = false;
    std::string coup_a_jouer = "";
    while (!gagnant)
    {
        std::cout << "Tour du joueur " << joueur_actuel << std::endl;
        plateau->affichagePlateau();
        parseMessage();
        std::cin >> coup_a_jouer;
        std::system("clear");

        if (jouerCoup(coup_a_jouer))
        {
            gagnant = FinPartie();
            joueur_actuel = (joueur_actuel + 1) % (nbJoueur + 1);
            if (joueur_actuel == 0)
            {
                joueur_actuel++;
            }
        }
    }
}

void JeuItem::deplacerPion(Point<> initial, Point<> final)
{
    // deplacement du pion
    plateau->getPlateau()[final.y][final.x]->setPionCase(plateau->getPlateau()[initial.y][initial.x]->getPionCase());
    plateau->getPlateau()[final.y][final.x]->getPionCase()->setPosition(final.x, final.y);
    plateau->getPlateau()[initial.y][initial.x]->setPionCase(nullptr);
    if(dynamic_cast<CaseItem*>(plateau->getPlateau()[final.y][final.x])->isItem()){
        dynamic_cast<CaseItem*>(plateau->getPlateau()[final.y][final.x])->getItem()->setPosition(nullptr);
        dynamic_cast<CaseItem*>(plateau->getPlateau()[final.y][final.x])->getPionCase()->addItem(dynamic_cast<CaseItem*>(plateau->getPlateau()[final.y][final.x])->getItem());
        dynamic_cast<PlateauItem*>(plateau.get())->supprimerItemPlateau(dynamic_cast<CaseItem*>(plateau->getPlateau()[final.y][final.x])->getItem());
        dynamic_cast<CaseItem*>(plateau->getPlateau()[final.y][final.x])->setItem(nullptr);
    }
}

//pq diff
bool JeuItem::poserMur(Point<> position_case1, Point<> position_case2)
{
    bool res = true;
    Pion *pion_actuel;
    for (auto &p : plateau->getPions())
    {
        pion_actuel = p;
        if (p->getNumerojoueur() == joueur_actuel && p->getNbrMurs() > 0)
        {
            

            if (isNeighbour(position_case1, position_case2))
            {
                if(!plateau->poserMurs(p, position_case1, position_case2)){
                    res = false;
                }
            }
        }
    }
    if(pion_actuel->getNbrMurs() <= 0){
        res = false;
    }
    for (auto &p : plateau->getPions())
    {
        if (!ExistChemin(p->getPosition()))
        {
            res = false;
            plateau->supprimerMurs();
            pion_actuel->setNbrMurs(pion_actuel->getNbrMurs() + 1);
            break;
        }
    }
    return res;
}

std::string JeuItem::parseMessage()
{
    std::string etatPlateau = "1"; // un message concernant le plateau doit obligatoirement commencer par 1

    // ajout de tout les pions
    for (auto &p : plateau->getPions())
    {
        etatPlateau.append(to_string(1));
        etatPlateau.append(to_string(p->getPosition().x));
        etatPlateau.append(to_string(p->getPosition().y));
    }
    if (getNbrJoueur() == 2)
    {
        etatPlateau.append(to_string(0));
        etatPlateau.append(to_string(0));
    }

    //ajout du nombre de murs par pions
    for(auto &p: plateau->getPions()){
        etatPlateau.append(to_string(p->getNbrMurs()));
        etatPlateau.append("n");
    }
    if (getNbrJoueur() == 2)
    {
        etatPlateau.append(to_string(0));
        etatPlateau.append("n");
        etatPlateau.append(to_string(0));
        etatPlateau.append("n");
    }

    // ajout de tout les murs dans le jeu
    etatPlateau.append(to_string(plateau->getMurs().size()));
    etatPlateau.append("m");
    for (auto &m : plateau->getMurs())
    {
        etatPlateau.append(to_string(m->getPositionMur().first.x));
        etatPlateau.append(to_string(m->getPositionMur().first.y));
        etatPlateau.append(to_string(m->getPositionMur().second.x));
        etatPlateau.append(to_string(m->getPositionMur().second.y));
    }

    //ajout des items presents sur le tableau
    etatPlateau.append(to_string(dynamic_cast<PlateauItem*>(plateau.get())->getVectorItems().size()));
    etatPlateau.append("i");
    for(auto &i: dynamic_cast<PlateauItem*>(plateau.get())->getVectorItems()){
        etatPlateau.append(to_string(i->getPosition()->x));
        etatPlateau.append(to_string(i->getPosition()->y));
        if(i->getitem() == "traverser"){
            etatPlateau.append("t");
        }else if(i->getitem() == "supprimer"){
            etatPlateau.append("s");
        }
        
    }

    //ajout des items possedé par les pions
    for(auto &p: plateau->getPions()){
        etatPlateau.append(to_string(dynamic_cast<PionItem*>(p)->getItems().size()));
        etatPlateau.append("p");
        if(dynamic_cast<PionItem*>(p)->getItem("traverser")){
            etatPlateau.append("t");
        }
        if(dynamic_cast<PionItem*>(p)->getItem("supprimer")){
            etatPlateau.append("s");
        }
    }
    if (getNbrJoueur() == 2)
    {
        etatPlateau.append(to_string(0));
        etatPlateau.append("p");
        etatPlateau.append(to_string(0));
        etatPlateau.append("p");
    }

    etatPlateau.append(to_string(joueur_actuel));
    etatPlateau.append(to_string(1));

    std::cout << etatPlateau << std::endl;
    return etatPlateau;
}

bool JeuItem::jouerCoup(std::string coup)
{
    std::string p = "p";
    std::string m = "m";
    std::string mode_coup = coup.substr(0, 1);
    Point<> position_init = coupToPoint(coup.substr(1, 3));
    Point<> position_final = coupToPoint(coup.substr(3, 5));
    if (mode_coup == "t")
    {
    }
    else if (mode_coup == "s")
    {
        for (auto &p : plateau->getPions())
        {
            if (p->getNumerojoueur() == joueur_actuel)
            {
                if(dynamic_cast<PionItem*>(p)->getItem("supprimer"))
                    if(dynamic_cast<PlateauItem*>(plateau.get())->supprimerMurItem(position_init, position_final)){
                        dynamic_cast<PionItem*>(p)->supprimerItem("supprimer");
                    }
            }
        }
    }
    if (mode_coup == p)
    {
        if (verifMouvement(position_init, position_final))
        {
            
            deplacerPion(position_init, position_final);
            return true;
        }
    }
    else if (mode_coup == m)
    {
        if (isNeighbour(position_init, position_final))
        {
            return poserMur(position_init, position_final);
        }
    }
    return false;
}

bool JeuItem::verifMouvement(Point<> position_init, Point<> position_final)
{
    // Verifie si la position initiale est correcte
    if (plateau->getPlateau()[position_init.y][position_init.x]->getPionCase() != nullptr && plateau->getPlateau()[position_init.y][position_init.x]->getPionCase()->getNumerojoueur() == joueur_actuel && plateau->getPlateau()[position_final.y][position_final.x]->getPionCase() == nullptr)
    {
        if (isNeighbour(position_init, position_final) && (dynamic_cast<PionItem*>(plateau->getPlateau()[position_init.y][position_init.x]->getPionCase())->getItem("traverser") || !plateau->isMur(position_init, position_final)))
        {
            if(dynamic_cast<PionItem*>(plateau->getPlateau()[position_init.y][position_init.x]->getPionCase())->getItem("traverser") && plateau->isMur(position_init, position_final)){
                dynamic_cast<PionItem*>(plateau->getPlateau()[position_init.y][position_init.x]->getPionCase())->supprimerItem("traverser");
            }
            return true;
        }
    }
    return false;
}