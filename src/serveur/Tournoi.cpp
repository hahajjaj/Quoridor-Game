# include "Tournoi.hpp"

void Tournoi::finPartie(User *u){
    for (auto p:parties){
        for (auto user:p->users){
            if (u == user){
                User *vainceur = p->users[p->joueur_gagnant];
                parties.erase(std::find(parties.begin(), parties.end(), p));
                addPlayer(vainceur);
                return ;
            }
        }
    }
}

void Tournoi::addPlayer(User *u){
    if (parties.size() == 0 || parties.back()->nbJoueurPret() == 2){
        cout << "nouvelle Partie" << endl;
        parties.push_back(make_shared<Partie>(u, serveur, 2));
        parties.back()->ModeJeu = "Tournoi";
        parties.back()->addUser(u);
        // serveur->sendTo(u, "0En attente d'un joueur");
    }
    else{
        cout << "ajout a Partie" << endl;
        parties.back()->setReady();
        parties.back()->addUser(u);
        parties.back()->play();
    }
}

bool Tournoi::newRound(){
    cout << "check next round" << endl;
    if (finiRound()){
        cout << "fini round" << endl;
        vector <User *> vainceurs;
        for (auto p:parties){
            int g = p->joueur_gagnant;
            vainceurs.push_back(p->users[g]);
            if (p->nbJoueurPret() == 2){
                if (!partisFinie.empty())
                    serveur->sendTo(p->users[g], "3Attente de la fin du round");
                cout << "envoie msg perdu" << endl;
                serveur->sendTo(p->users[1-g], "2Vous avez perdu la partie");
                p->users[1-g]->setPartie(nullptr);
                p->users[1-g]->clearTournoi();
            }
        }
        if (vainceurs.size() == 1 && partisFinie.empty()){
            cout << "fin tournoi" << endl;
            vainceurs[0]->setPartie(nullptr);
            vainceurs[0]->clearTournoi();
            serveur->sendTo(vainceurs[0], "2Vous avez gagné le tournoi");
            return false;
        }
        parties.clear();
        partisFinie.clear();
        for (auto v : vainceurs){
            cout << "envoie msg gagnant attent autre partie" << endl;
            addPlayer(v);
        }
    }
    else
    {
        vector <User *> vainceurs;
        for (auto it = parties.begin(); it != parties.end();){
            shared_ptr<Partie> p = *it;
            if (!p->isFinished())
            {
                ++it;
                continue;
            }
            int g = p->joueur_gagnant;
            vainceurs.push_back(p->users[g]);
            if (p->nbJoueurPret() == 2){
                cout << "envoie msg perdu" << endl;
                serveur->sendTo(p->users[g], "3Attente de la fin du round");
                serveur->sendTo(p->users[1-g], "2Vous avez perdu la partie");
                p->users[1-g]->setPartie(nullptr);
                p->users[1-g]->clearTournoi();
                partisFinie.push_back(p);
                it = parties.erase(it);
            }
        }
        for (auto v : vainceurs){
            cout << "envoie msg gagnant attent autre partie" << endl;
            // serveur->sendTo(v, "3Attente de la fin du round");
            addPlayer(v);
        }
    }
    return true;
}

bool Tournoi::finiRound(){
    for (auto p: parties){
        if (!p->isFinished() && p->nbJoueurPret() == 2){
            return false;
        }
    }
    return true;
}