#ifndef USER_HPP
# define USER_HPP
///////////////////// USER ///////////////////////////////////////////////
#include "Partie.hpp"
#include "Tournoi.hpp"
class Tournoi;
class User
{
    int fd = 0;
    bool logged = false;
    Partie *partie=nullptr;
    shared_ptr<Tournoi> tournoi;
    string username = "";
    bool gui = false;
    string password = "";
    string chattingWith = "";
    string status = "";
    enum invitationType{NORMAL, TOURNOIS, REPRENDREPARTIE};
    struct invit{string pseudo; invitationType type;};
    vector <invit> invitations;
public:

    User(int fd) : fd{fd} {}
    User(const User &src){ fd = src.fd; }
    User(User &&src):fd{src.fd}{ src.fd = -1; }

    ~User(){close(fd);}
    void closeFd();

    string getUsername(){ return username; }
    void setUsername(string u){ username = u; }

    string getPassword(){ return password; }
    void setPassword(string u){ password = u; }

    string getChattingWith(){ return chattingWith; }
    void setChattingWith(string u){ chattingWith = u; }

    bool inTournoi(){ return tournoi != nullptr; }
    shared_ptr<Tournoi> getTournoi(){ return tournoi; }
    void setTournoi(shared_ptr<Tournoi> otherTournoi){ tournoi = otherTournoi; }
    void setTournoi(Serveur* otherTournoi);
    void clearTournoi(){ tournoi = nullptr; }

    bool isGui() const { return gui; }
    void setGui(bool _gui) { gui = _gui; }

    bool isInGame(){ return (partie != nullptr && partie->jeu != nullptr); }

    void statusClear(){ status.clear(); }
    string getStatus() { return status; }
    void setStatus(string s) { status = s; }
    void statusAdd(string s) { status += s; }
    bool statusIs(string s) { return status == s; }

    void login(string n, string p);

    Partie *getPartie() { return partie; }
    void setPartie(Partie *p){ partie = p; }
    int getFd() { return fd; }
    bool isLogged() { return logged; }
    void setLogged() { logged = !logged; }
    void reset();

    bool hasSendInviteTo(string pseudo);
    void removeAllInvitationGame();
    string showInvitation();

    void sendInvitationGame(string pseudo, invitationType type=NORMAL);
    void removeInvitationGame(string pseudo, invitationType type=NORMAL);

    void sendSauvegardeGame(string pseudo1, string pseudo2, string etat_plateau);

    void sendSauvegardeGame(vector<string> pseudos, string etat_plateau);

    void sendSauvegardeGame(string pseudo1, string pseudo2, string pseudo3, string pseudo4, string etat_plateau);

    string removeSauvegardeGame(string liste_pseudo);

    string getSauvegardePartie(string pseudo);


    int taille(string str);

    vector<string> separateur(string str, char seperator);
};

#endif