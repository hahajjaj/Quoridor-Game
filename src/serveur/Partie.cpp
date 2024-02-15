#include "Partie.hpp"
#include "Serveur.hpp"

Partie::Partie(User *user, Serveur *s,int nbJoueur):srv{s},nbJoueur{nbJoueur}{
    cout << "creation partie" << endl;
    user->setPartie(this);
}

Partie::~Partie(){
    cout << "destruction partie" << endl;
    int len = users.size();
    for (int i = 0; i < len; i ++){
        users.pop_back();
    }
}

void Partie::lancePartie(string plateau){
    cout << "CREER INSTANCE DE JEU, LANCER PARTIE AVEC PLATEAU" << endl;
    jeu = make_unique<Jeu>(nbJoueur, this, plateau);
}


void Partie::lancePartie(){
    cout << "CREER INSTANCE DE JEU, LANCER PARTIE " << endl;
    if (ModeJeu == "Item"){
        jeu = make_unique<JeuItem>(nbJoueur, this, new PlateauItem(9, nbJoueur));
    }
    else
    {
        jeu = make_unique<Jeu>(nbJoueur, this, new Plateau(9, nbJoueur));
    }
}


// Retourne true si tous les joueurs sont prêts 
bool Partie::joueurPret(){
    if (nbJoueursConnected == nbJoueur && readyBool) return true;
    return false;
}

void Partie::addUser(User *user){
    cout << "ajout du joueur: " << user->getUsername() << endl;
    user->setPartie(this);
    users.push_back(user);
    nbJoueursConnected += 1;
}


void Partie::setNbJoueur(int j){nbJoueur = j;}


void Partie::play(string plateau){
    cout << " PLAY PLATEAU " << endl;
    if (joueurPret()){
       
        lancePartie(plateau);
        sendAllBoard(jeu->parseMessage());
    }
}

void Partie::play(){
    cout << " PLAY" << endl;
    if (joueurPret()){
        lancePartie();
        sendAllBoard(jeu->parseMessage());
    }
}

bool Partie::isFinished(){return (jeu && jeu->FinPartie());}

void Partie::sendAll(string message){

    for (auto u : users){
        srv->ssend(u->getFd(), message.c_str(), message.size()+1);
    }
}

void Partie::sendAllBoard(string message)
{
    for (size_t i = 0; i < users.size(); ++i)
        srv->ssend(users[i]->getFd(), (message + to_string(i)).c_str(), message.size()+2);
}

bool Partie::jouerCoup(char *message){
    bool coup_jouer = jeu->getMessage(message);
    turn_to_play +=1;

    return coup_jouer;
}

// Renvoi le joueur qui est censé jouer
User* Partie::getUser(){
    cout << "tour du joueur: " << jeu->getJoueurActuel() - 1 << " de jouer " << users[turn_to_play % nbJoueur]->getUsername() << endl;
    return users[jeu->getJoueurActuel() - 1];

} 

int Partie::nbJoueurPret(){
    return users.size();
}
