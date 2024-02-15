#ifndef TOURNOI_HPP
# define TOURNOI_HPP
# include "../common/common.hpp"
# include "Partie.hpp"
# include "Serveur.hpp"
class Tournoi
{
private:
    /* data */
    Serveur *serveur;
    vector <shared_ptr<Partie>> parties;
    vector<shared_ptr<Partie>> partisFinie;
    User *creator = nullptr;
public:
    Tournoi(Serveur *s):serveur{s}{}
    virtual ~Tournoi() {cout << "Destructeur Tournoi" << endl;}
    void addPlayer(User *u);
    void finPartie(User *u);
    bool finiRound();
    bool newRound();
    User *getCreator() const {return this->creator;}
    void setCreator(User *u) {this->creator = u;}
};

#endif