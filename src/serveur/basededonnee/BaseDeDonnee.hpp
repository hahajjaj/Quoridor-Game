#ifndef __BASE_DE_DONNE_HPP
# define __BASE_DE_DONNE_HPP

// #include "../../common/common.hpp"

// g++ -std=c++2a BaseDeDonnee

#include "../../common/common.hpp"


using namespace std;

int getTime();

class BaseDeDonnee
{
public:
    struct Classement
    {
        inline static const char *fileName = "classement.txt";
        inline static const char *tmp = "tmpClass.txt";
        string pseudo = "";
        int score = -1, timestamp = -1;
    };
    struct Friend
    {
        inline static const char *fileName = "friends.txt";
        inline static const char *tmp = "tmpFriends.txt";
        string pseudo1, pseudo2;
    };
    struct Chat
    {
        inline static const char *fileName = "chat.txt";
        inline static const char *tmp = "tmpChat.txt";
        string pseudo1, pseudo2, chat;
    };
private:
    BaseDeDonnee(/* args */);
public:
    BaseDeDonnee(const BaseDeDonnee &db) = delete;
    BaseDeDonnee(const BaseDeDonnee &&db) = delete;
    BaseDeDonnee &operator=(const BaseDeDonnee &db) = delete;
    BaseDeDonnee &operator=(const BaseDeDonnee &&db) = delete;
    static shared_ptr<BaseDeDonnee> getInstance();
    ~BaseDeDonnee();


    void update(const string &pseudo, int score, int timestamp = getTime());
    int getScore(const string &pseudo);
    string getGlobalClass();
    string getFriendsClass(const string &pseudo);
    string getHebdoClass(int since = getTime() - (60 * 60 * 24 * 7));


    bool areFriend(const string &pseudo1, const string &pseudo2);
    string addFriend(string pseudo1, string pseudo2);
    string delFriend(string pseudo1, string pseudo2);
    string demandeFriend(string pseudo1, string pseudo2);
    string refuseDemande(string pseudo1, string pseudo2);
    string getFriends(string pseudo);
    string getFriendsAttente(string pseudo);

    void setChat(string pseudo1, string pseudo2, string chat = "");
    string getChat(string pseudo1, string pseudo2);

    // logon / sign in
    bool exist(const string &username);
    bool check_exist(const string &username, const string &password);
    bool create(const string &username, const string &password);
    bool login(const string &username, const string &password);


    // void modifieDataBase(new data);
    // string *getFromDataBase(condition);




};

ostream &operator<<(ostream &os, BaseDeDonnee::Classement &clss);
istream &operator>>(istream &is, BaseDeDonnee::Classement &clss);
ostream &operator<<(ostream &os, BaseDeDonnee::Friend &frd);
istream &operator>>(istream &is, BaseDeDonnee::Friend &frd);
ostream &operator<<(ostream &os, BaseDeDonnee::Chat &chat);
istream &operator>>(istream &is, BaseDeDonnee::Chat &chat);

#endif
