#include "../header/Jeu.hpp"
#include "../../../Serveur.hpp"



//constructeur pour le serveur
Jeu::Jeu(int nombreJoueur, Serveur *serveur) : nbJoueur{nombreJoueur}
{
    createGame(serveur);
}
Jeu::Jeu(int nombreJoueur, Partie *partie,  Plateau *p) : nbJoueur{nombreJoueur}, plateau{unique_ptr<Plateau>(p)}, partie{partie}{
}

Jeu::Jeu(int nombreJoueur, Partie *partie, string plt) : nbJoueur{nombreJoueur}, plt{plt}, partie{partie}{
    // a partir de plt creer plateau

    plateau = make_unique<Plateau>(9, nbJoueur);
    joueur_actuel = plateau->String_to_plateau(plt,nombreJoueur);
}

// constructeur pour les testes
Jeu::Jeu(int nombreJoueur) : nbJoueur{nombreJoueur}
{
    plateau = make_unique<Plateau>(9, nbJoueur);
    
}

// destructeur
Jeu::~Jeu()
{   
}

// Initialise une partie
void Jeu::createGame(Serveur *serveur)
{
    plateau = make_unique<Plateau>(9, nbJoueur);
}

// methode appelé par le serveur pour jouer un coup et récupérer l'état du plateau
bool Jeu::getMessage(char *message)
{
    
    bool coup_joue = jouerCoup(message);

    if (coup_joue)
    {
        cout << "Le coup est bien valide" << endl;
        joueur_actuel = (joueur_actuel + 1) % (nbJoueur+1);
        if (joueur_actuel == 0)
        {
            joueur_actuel++;
        }
    }
    partie->sendAllBoard(parseMessage());

    if (FinPartie()){
        string winner = "3Le joueur " + partie->users[gagnant-1]->getUsername() + " a gagné la partie !";
        // partie->sendAll(winner + "\x1f\n" + "1. Jouer\n2. Amis\n3. Règles du jeu\n4. Classement \n5.Déconnexion");
        // partie->isFinished = true;
        partie->joueur_gagnant = gagnant - 1;
    }

    return coup_joue;
}

// methode qui permet de parser l'état du plateau en un string pour pouvoir le communiquer au serveur
std::string Jeu::parseMessage()
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

    etatPlateau.append("0i0p0p0p0p");

    etatPlateau.append(to_string(joueur_actuel));
    etatPlateau.append(to_string(0));

    std::cout << etatPlateau << std::endl;
    return etatPlateau;
}

// Boucle générale du jeu
void Jeu::run()
{
    bool gagnant = false;
    std::string coup_a_jouer = "";
    while (!gagnant)
    {
        std::cout << "Tour du joueur " << joueur_actuel << std::endl;
        plateau->affichagePlateau();
        std::cin >> coup_a_jouer;
        std::system("clear");

        if (jouerCoup(coup_a_jouer))
        {
            gagnant = FinPartie();
            cout << "Le gagnant est : " << gagnant << endl;
            joueur_actuel = (joueur_actuel + 1) % (nbJoueur+1);

            if (joueur_actuel == 0)
            {
                joueur_actuel++;
            }
        }
    }
}

bool Jeu::jouerCoup(std::string coup)
{
    std::string p = "p";
    std::string m = "m";
    std::string mode_coup = coup.substr(0, 1);
    Point<> position_init = coupToPoint(coup.substr(1, 3));
    Point<> position_final = coupToPoint(coup.substr(3, 5));
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

void Jeu::deplacerPion(Point<> initial, Point<> final)
{
    // deplacement du pion
    plateau->getPlateau()[final.y][final.x]->setPionCase(plateau->getPlateau()[initial.y][initial.x]->getPionCase());
    plateau->getPlateau()[final.y][final.x]->getPionCase()->setPosition(final.x, final.y);
    plateau->getPlateau()[initial.y][initial.x]->setPionCase(nullptr);
}

bool Jeu::poserMur(Point<> position_case1, Point<> position_case2)
{
    bool res = true;
    Pion *pion_actuel;
    for (auto &p : plateau->getPions())
    {
        if(p->getNumerojoueur() == joueur_actuel && p->getNbrMurs() <= 0){
            res = false;
        }
        if (p->getNumerojoueur() == joueur_actuel && p->getNbrMurs() > 0)
        {
            pion_actuel = p;
            if (isNeighbour(position_case1, position_case2))
            {
                if(!plateau->poserMurs(p, position_case1, position_case2)){
                    res = false;
                }
            }
        }
        
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

// convertit un coup (string) en une structure Point
Point<> Jeu::coupToPoint(std::string coup)
{
    int x = ((coup[1] - '0') - 1);
    int y = ((int)coup[0] - 97);
    Point<> p{x, y};
    return p;
}

bool Jeu::verifMouvement(Point<> position_init, Point<> position_final)
{
    // Verifie si la position initiale est correcte
    if (plateau->getPlateau()[position_init.y][position_init.x]->getPionCase() != nullptr && plateau->getPlateau()[position_init.y][position_init.x]->getPionCase()->getNumerojoueur() == joueur_actuel && plateau->getPlateau()[position_final.y][position_final.x]->getPionCase() == nullptr)
    {
        if (isNeighbour(position_init, position_final) && !plateau->isMur(position_init, position_final))
        {
            return true;
        }
    }
    return false;
}

bool Jeu::isNeighbour(Point<> pos_init, Point<> pos_fin)
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (!(i == 0 && j == 0) && (abs(i) != abs(j)) && plateau->respecteDelimitation(pos_init.y + i, pos_init.x + j) && pos_fin.x == (pos_init.x + j) && pos_fin.y == (pos_init.y + i))
            {
                return true;
            }
        }
    }
    return false;
}

bool Jeu::FinPartie()
{
    bool res = false;
    int len = plateau->getTaille() - 1;

    // parcourt de la derniere ligne pour joueur 1
    for (int x = 0; x < 9; x++)
    {
        if (plateau->getPlateau()[len][x]->getPionCase() != nullptr)
        {
            if (plateau->getPlateau()[len][x]->getPionCase()->getNumerojoueur() == 1)
            {
                res = true;
                gagnant = 1;
            }
        }
    }

    // parcourt de la premiere ligne pour joueur 2
    for (int x = 0; x < 9; x++)
    {
        if (plateau->getPlateau()[0][x]->getPionCase() != nullptr)
        {
            if (plateau->getPlateau()[0][x]->getPionCase()->getNumerojoueur() == 2)
            {
                res = true;
                gagnant = 2;
            }
        }
    }
    if (nbJoueur == 4)
    {
        // parcourt de la dernière colonne pour joueur 3
        for (int y = 0; y < 9; y++)
        {
            if (plateau->getPlateau()[y][len]->getPionCase() != nullptr)
            {
                if (plateau->getPlateau()[y][len]->getPionCase()->getNumerojoueur() == 3)
                {
                    res = true;
                    gagnant = 3;
                }
            }
        }

        // parcourt de la première colonne pour joueur 4
        for (int y = 0; y < 9; y++)
        {
            if (plateau->getPlateau()[y][0]->getPionCase() != nullptr)
            {
                if (plateau->getPlateau()[y][0]->getPionCase()->getNumerojoueur() == 4)
                {
                    res = true;
                    gagnant = 4;
                }
            }
        }
    }

    if (res == true)
    {
        std::cout << "FIN DE PARTIE "
                  << " - "
                  << "Joueur : " << gagnant << "gagnant !" << std::endl;
    }
    return res;
}

bool Jeu::ExistChemin(Point<> position)
{
    // directions
    int dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    // création d'une queue
    queue<pair<int, int>> q;

    // detection du coté à tester
    Point<> pion_init = plateau->getPlateau()[position.y][position.x]->getPionCase()->getPositionInitial();
    int destination;
    bool axe_x = false;
    if (pion_init.x == 0 || pion_init.y == 0)
    {
        if (pion_init.y == 0)
        {
            axe_x = true;
        }
        destination = plateau->getTaille() - 1;
    }
    else if (pion_init.x == plateau->getTaille() - 1 || pion_init.y == plateau->getTaille() - 1)
    {
        if (pion_init.y == plateau->getTaille() - 1)
        {
            axe_x = true;
        }
        destination = 0;
    }
    // inserer la position du pion
    q.push(make_pair(position.x, position.y));

    // Tant que la queue n'est pas vide
    while (q.size() > 0)
    {
        pair<int, int> p = q.front();
        q.pop();

        // Marque une case comme étant déjà visité
        plateau->getPlateau()[p.second][p.first]->setVisited(-1);

        // Nous avons atteint la destination
        if (axe_x)
        {
            if (p.second == destination)
            {
                plateau->remettreVisiteAZero();
                return true;
            }
        }
        else
        {
            if (p.first == destination)
            {
                plateau->remettreVisiteAZero();
                return true;
            }
        }

        // Ici on teste les 4 directions
        for (int i = 0; i < 4; i++)
        {

            int a = p.first + dir[i][0];
            int b = p.second + dir[i][1];

            // rajoute a la queue les nouvelles cases a tester
            if (plateau->respecteDelimitation(a, b) && !plateau->isMur(Point<>{p.first, p.second}, Point<>{a, b}) && plateau->getPlateau()[b][a]->getVisited() != -1 && a >= 0 && b >= 0 && a < plateau->getTaille() && b < plateau->getTaille())
            {
                q.push(make_pair(a, b));
            }
        }
    }
    plateau->remettreVisiteAZero();
    return false;
}

std::string Jeu::parsePlateauString()
{
    std::string plateauString = "5";
    plateauString.append("Joueur actuel : ");
    plateauString.append(to_string(joueur_actuel));
    plateauString.append("\n");
    std::string lettres = "abcdefghi";
    for (int y = 0; y < plateau->getTaille(); y++)
    {
        std::string s;
        s.append(1, lettres[y]);
        plateauString.append(s);
        plateauString.append(" ");
        for (int x = 0; x < plateau->getTaille(); x++)
        {
            plateauString.append(" ");
            if (plateau->getPlateau()[y][x]->getPionCase() == nullptr)
            {
                plateauString.append(".");
            }
            else
            {
                if (plateau->getPlateau()[y][x]->getPionCase()->getNumerojoueur() == 1)
                {
                    plateauString.append("a");
                }
                else if (plateau->getPlateau()[y][x]->getPionCase()->getNumerojoueur() == 2)
                {
                    plateauString.append("g");
                }
                if (plateau->getPlateau()[y][x]->getPionCase()->getNumerojoueur() == 3)
                {
                    plateauString.append("n");
                }
                else if (plateau->getPlateau()[y][x]->getPionCase()->getNumerojoueur() == 4)
                {
                    plateauString.append("s");
                }
            }

            if (x != plateau->getTaille() - 1)
            {
                if (plateau->isMur(Point<>{x, y}, Point<>{x + 1, y}))
                {
                    plateauString.append(" x");
                }
                else
                {
                    plateauString.append("  ");
                }
            }
        }
        plateauString.append("\n");
        plateauString.append(" ");
        if (y != plateau->getTaille() - 1)
        {
            for (int i = 0; i < plateau->getTaille(); i++)
            {
                if (plateau->isMur(Point<>{i, y}, Point<>{i, y + 1}))
                {
                    plateauString.append("  x ");
                }
                else
                {
                    plateauString.append("    ");
                }
            }
            plateauString.append("\n");
        }
    }
    plateauString.append(" ");


    std::cout << std::endl << " ";
    plateauString.append("\n");
    plateauString.append(" ");
    for (int i = 0; i < plateau->getTaille(); i++)
    {
        plateauString.append("  ");
        plateauString.append(to_string(i + 1));
        plateauString.append(" ");
    }
    plateauString.append("\n");
    std::cout << plateauString << endl;
    return plateauString;
}

void Jeu::setJoueurActuel(int new_joueur)
{
    joueur_actuel = new_joueur;
}