#include "../header/Plateau.hpp"
#include "../../../Serveur.hpp"

Plateau::Plateau(int p_taille, int nombreJoueur, Serveur *serveur) : taille{p_taille}, nbrJoueur{nombreJoueur}, serveur{serveur}
{
    initializePLateau();
    initializePion();
}

Plateau::Plateau(int p_taille, int nombreJoueur, bool isItem) : taille{p_taille}, nbrJoueur{nombreJoueur}{
    // n'initialise rien pour permettre au jeu item d'initialiser son propre plateau
}

Plateau::Plateau(int p_taille, int nombreJoueur) : taille{p_taille}, nbrJoueur{nombreJoueur}
{
    initializePLateau();
    initializePion();
}

Plateau::~Plateau()
{
    for (auto &m : murs)
    {
        delete m;
        m = nullptr;
    }

    for (auto &v : plateau)
    {
        for (auto &c : v)
        {
            delete c;
            c = nullptr;
        }
    }
}

void Plateau::initializePLateau()
{
    for (int y = 0; y < taille; y++)
    {
        plateau.push_back({});
        for (int x = 0; x < taille; x++)
        {
            Case *c = new Case(x, y);
            plateau[y].push_back(c);
        }
    }
}

void Plateau::initializePion()
{
    Pion *p = new Pion(4, 0, 1);
    Pion *p2 = new Pion(4, 8, 2);
    pions.push_back(p);
    pions.push_back(p2);
    plateau[0][4]->setPionCase(p);
    plateau[8][4]->setPionCase(p2);
    if (nbrJoueur == 4)
    {
        p->setNbrMurs(5);
        p2->setNbrMurs(5);
        Pion *p3 = new Pion(0, 4, 3);
        Pion *p4 = new Pion(8, 4, 4);
        p3->setNbrMurs(5);
        p4->setNbrMurs(5);
        pions.push_back(p3);
        pions.push_back(p4);
        plateau[4][0]->setPionCase(p3);
        plateau[4][8]->setPionCase(p4);
    }
}

// Passer du string contenant l'etat plateau a un objet plateau (sans items)
int Plateau::String_to_plateau(std::string etat_plateau, int nbrJoueurs)
{
    cout << "String to plateau rentrer " << endl;
    nbrJoueur = nbrJoueurs;
 
    // pions.clear();

    for (auto &m : murs){
        delete m;
    }
    murs.clear();
    
    // vide le plateau
    for (int y = 0; y < taille; y++)
    {
        for (int x = 0; x < taille; x++)
        {

            // plateau[y][x]->viderCase();
        }
    }

    char *ptr = (char *)etat_plateau.c_str();
    for (int i = 1; i <= 4; i++)
    {
        for (int nbPions = *ptr++ - '0'; nbPions > 0; nbPions--)
        {
            int x = *ptr++ -'0';
            int y = *ptr++ - '0';
            Pion *p = pions[i - 1];
            auto pos =  p->getPosition();
            plateau[pos.y][pos.x]->viderCase();
            p->setPosition(x, y);
            plateau[y][x]->setPionCase(p);
        }
    }
    string buff = "";
    // recupere le nb de murs pour chaque joueur
    for (int i = 0; i < 4; ++i)
    {
        buff.clear();
        while (*ptr != 'n')
        {
            buff += *ptr++;
        }
        ++ptr;
        if (i < nbrJoueur)
            pions[i]->setNbrMurs(stoi(buff));
    }
    buff.clear();

    while (*ptr != 'm')
        buff += *ptr++;
    ++ptr;
    // recupere les positions des murs déjà posé sur le plateau
    for (int nbWall = stoi(buff); nbWall > 0; --nbWall)
    {
        Mur *m = new Mur(Point<>{*ptr++ -'0',*ptr++ -'0'}, Point<>{*ptr++ -'0',*ptr++ -'0'});
        murs.push_back(m);
    }

    buff.clear();
    while (*ptr != 'i')
        buff += *ptr++;
    ++ptr; // recupere le nb d'items
    for (int nbItem = stoi(buff); nbItem > 0; --nbItem)
    {
        ptr++; ptr++;
    }

    // recupere le nb d'items pour chaque joueur
    for (int i = 0; i < 4; ++i)
    {
        buff.clear();
        while (*ptr != 'p')
            buff += *ptr++;
        ++ptr;
        for (int nbItem = stoi(buff); nbItem > 0; --nbItem)
            ptr++;
    }

    return static_cast<int>(*ptr++ - '0');
}

void Plateau::afficherCase(int y, int x){
    if (plateau[y][x]->getPionCase() == nullptr)
    {
        std::cout << ".";
    }
    else
    {
        if (plateau[y][x]->getPionCase()->getNumerojoueur() == 1)
        {
            std::cout << "•";
        }
        else if (plateau[y][x]->getPionCase()->getNumerojoueur() == 2)
        {
            std::cout << "✱";
        }
        if (plateau[y][x]->getPionCase()->getNumerojoueur() == 3)
        {
            std::cout << "n";
        }
        else if (plateau[y][x]->getPionCase()->getNumerojoueur() == 4)
        {
            std::cout << "s";
        }
    }
}

void Plateau::affichagePlateau()
{
    std::string lettres = "abcdefghi";
    for (int y = 0; y < taille; y++)
    {
        std::cout << lettres[y] << " ";
        for (int x = 0; x < taille; x++)
        {
            std::cout << " ";
            afficherCase(y, x);
            if (x != taille - 1)
            {
                if (isMur(Point<>{x, y}, Point<>{x + 1, y}))
                {
                    std::cout << " x";
                }
                else
                {
                    std::cout << "  ";
                }
            }
        }
        std::cout << std::endl;
        std::cout << " ";
        if (y != taille - 1)
        {
            for (int i = 0; i < taille; i++)
            {
                if (isMur(Point<>{i, y}, Point<>{i, y + 1}))
                {
                    std::cout << "  x ";
                }
                else
                {
                    std::cout << "    ";
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << " ";

    std::cout << std::endl
              << " ";
    for (int i = 0; i < taille; i++)
    {
        std::cout << "  " << i + 1 << " ";
    }
    std::cout << std::endl;
}

bool Plateau::poserMurs(Pion *p, Point<> position1, Point<> position2)
{
    Mur *m = new Mur(position1, position2);
    // placement d'un mur horizontalement
    if (position1.x == position2.x && position1.x != taille - 1)
    {
        if (!isMur(position1, position2) && !isMur(Point<>{position1.x + 1, position1.y}, Point<>{position2.x + 1, position2.y}))
        {
            if (!(isMur(position1, Point<>{position1.x + 1, position1.y}) && isMur(position2, Point<>{position2.x + 1, position2.y})))
            {
                murs.push_back(m);
                Mur *m2 = new Mur(Point<>{position1.x + 1, position1.y}, Point<>{position2.x + 1, position2.y});
                murs.push_back(m2);
                p->setNbrMurs(p->getNbrMurs() - 1);
                return true;
            }
        }
    }
    // placement d'un mur verticalement
    else if (position1.y == position2.y && position1.y != taille - 1) // Placement d'un mur verticalement
    {
        if (!isMur(position1, position2) && !isMur(Point<>{position1.x, position1.y + 1}, Point<>{position2.x, position2.y + 1}))
        {
            if (!(isMur(position1, Point<>{position1.x, position1.y + 1}) && isMur(position2, Point<>{position2.x, position2.y + 1})))
            {
                murs.push_back(m);
                Mur *m2 = new Mur(Point<>{position1.x, position1.y + 1}, Point<>{position2.x, position2.y + 1});
                murs.push_back(m2);
                p->setNbrMurs(p->getNbrMurs() - 1);
                return true;
            }
        }
    }
    return false;
}

void Plateau::supprimerMurs()
{
    murs.pop_back();
    murs.pop_back();
}

bool Plateau::respecteDelimitation(int y, int x)
{
    return ((y >= 0 && y < taille) && (x >= 0 && x < taille));
}

std::vector<Mur *> Plateau::getMurs()
{
    return murs;
}


bool Plateau::isMur(Point<> pos_init, Point<> pos_fin)
{
    for (auto &m : murs)
    {
        if ((pos_init.x == m->getPositionMur().first.x && pos_init.y == m->getPositionMur().first.y) || (pos_init.x == m->getPositionMur().second.x && pos_init.y == m->getPositionMur().second.y))
        {
            if ((pos_fin.x == m->getPositionMur().first.x && pos_fin.y == m->getPositionMur().first.y) || (pos_fin.x == m->getPositionMur().second.x && pos_fin.y == m->getPositionMur().second.y))
            {
                return true;
            }
        }
    }
    return false;
}

void Plateau::remettreVisiteAZero()
{
    for (auto &i : plateau)
    {
        for (auto &c : i)
        {
            c->setVisited(0);
        }
    }
}
