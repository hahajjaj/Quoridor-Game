#include "BaseDeDonnee.hpp"

int getTime()
{
    return static_cast<int>(chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count());
}

BaseDeDonnee::BaseDeDonnee()
{
    // créé un fichier pour le classement s'il n'existe pas déjà
    ofstream file;
    file.open(Classement::fileName, ios::out | ios::app);
    file.close();
    file.open(Friend::fileName, ios::out | ios::app);
    file.close();
    file.open("invit.txt", ios::out | ios::app);
    file.close();
    file.open(Chat::fileName, ios::out | ios::app);
    file.close();
    ofstream file2{Friend::fileName, ios::out | ios::app};
    file.close();
    ofstream file3{"invit.txt", ios::out | ios::app};
    file.close();
}

shared_ptr<BaseDeDonnee> BaseDeDonnee::getInstance()
{
    static shared_ptr<BaseDeDonnee> instance{new BaseDeDonnee()};
    return instance;
}

BaseDeDonnee::~BaseDeDonnee()
{
}

void BaseDeDonnee::update(const string &pseudo, int score, int timestamp)
{
    ifstream inFile{Classement::fileName, ios::in};
    ofstream outFile{Classement::tmp, ios::out};

    if (!inFile || !outFile)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", inFile ? Classement::tmp : Classement::fileName);
        perror("");
        exit(1);
    }

    Classement newClass{pseudo, score, timestamp}, buffer;

    while (inFile >> buffer)
    {
        if (buffer.score <= newClass.score)
        {
            outFile << newClass << endl;
            newClass.score = -1000;
        }
        if (buffer.pseudo != newClass.pseudo)
            outFile << buffer << endl;
    }
    if (newClass.score == score)
        outFile << newClass;

    inFile.close();
    outFile.close();
    rename(Classement::tmp, Classement::fileName);
}

int BaseDeDonnee::getScore(const string &pseudo)
{
    ifstream inFile{Classement::fileName, ios::in};

    if (!inFile)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", Classement::fileName);
        perror("");
        exit(1);
    }
    Classement buffer;

    while (inFile >> buffer)
    {
        if (buffer.pseudo == pseudo)
        {
            inFile.close();
            return buffer.score;
        }
    }
    inFile.close();
    return 0;
}

string BaseDeDonnee::getGlobalClass()
{
    ifstream inFile{Classement::fileName, ios::in};
    if (!inFile)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", Classement::fileName);
        perror("");
        exit(1);
    }
    ostringstream clss{"", ios::out};

    Classement buffer;

    while (inFile >> buffer)
    {
        clss << buffer << endl;
    }

    inFile.close();
    return clss.str();
}

string BaseDeDonnee::getFriendsClass(const string &pseudo)
{
    ifstream inFile{Classement::fileName, ios::in};
    if (!inFile)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", Classement::fileName);
        perror("");
        exit(1);
    }
    ostringstream clss{"", ios::out};

    Classement buffer;

    while (inFile >> buffer)
    {
        if (areFriend(pseudo, buffer.pseudo))
            clss << buffer << endl;
    }
    inFile.close();
    return clss.str();
}

string BaseDeDonnee::getHebdoClass(int since)
{
    ifstream inFile{Classement::fileName, ios::in};
    if (!inFile)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", Classement::fileName);
        perror("");
        exit(1);
    }
    ostringstream clss{"", ios::out};

    Classement buffer;

    while (inFile >> buffer)
    {
        if (buffer.timestamp >= since)
            clss << buffer << endl;
    }
    inFile.close();
    return clss.str();
}

bool BaseDeDonnee::areFriend(const string &pseudo1, const string &pseudo2)
{   
    ifstream inFile{BaseDeDonnee::Friend::fileName, ios::in};
    if (!inFile)
    {   
        cerr << "Impossible d'ouvrir" << Friend::fileName;
        perror("");
        exit(1);
    }

    Friend buffer;
    while (inFile >> buffer)
    {   
        
        if ((pseudo1 == buffer.pseudo1 && pseudo2 == buffer.pseudo2) ||
            (pseudo1 == buffer.pseudo2 && pseudo2 == buffer.pseudo1))
        {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

string BaseDeDonnee::addFriend(string pseudo1, string pseudo2)
{
    if (pseudo1 == pseudo2)
        return "Vous ne pouvez pas vous ajouter vous même";
    if (areFriend(pseudo1, pseudo2))
        return "Vous êtes déjà amis avec" + pseudo2; // sont déjà amis
    ifstream inDemande{"invit.txt", ios::in};
    ofstream outDemande{"tmpInvit.txt", ios::out};
    if (!inDemande || !outDemande)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", outDemande ? "invit.txt" : "tmpInvit.txt");
        perror("");
        exit(1);
    }
    Friend buffer;
    int demande = 1;
    while (inDemande >> buffer)
    {
        if (buffer.pseudo1 == pseudo2 && buffer.pseudo2 == pseudo1)
            demande = 0;
        else if (buffer.pseudo1 == pseudo1 && buffer.pseudo2 == pseudo2)
            demande = 2;
        else
            outDemande << buffer << endl;
    }
    inDemande.close();
    outDemande.close();
    rename("tmpInvit.txt", "invit.txt");
    if (demande == 1)
        return pseudo2 + " ne vous à pas encore envoyé de demande d'amis"; // pseudo2 n'a pas envoyé de demande d'amis à pseudo1
    else if (demande == 2)
        return "Attendez que " + pseudo2 + " accepte votre demande d'amis";
    ofstream outFriend{Friend::fileName, ios::out | ios::app};
    if (!outFriend)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", "invit.txt");
        perror("");
        exit(1);
    }
    buffer.pseudo1 = pseudo1; buffer.pseudo2 = pseudo2;
    outFriend << buffer << endl;
    buffer.pseudo1.swap(buffer.pseudo2);
    outFriend << buffer << endl;
    outFriend.close();
    return "Done";
}

string BaseDeDonnee::delFriend(string pseudo1, string pseudo2)
{
    if (pseudo1 == pseudo2)
        return "Vous ne pouvez pas vous supprimer vous même de vos amis";
    if (!exist(pseudo2))
        return pseudo2 + " n'existe pas";
    ifstream inFriend{Friend::fileName, ios::in};
    ofstream outFriend{Friend::tmp, ios::out};
    if (!inFriend || !outFriend)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", inFriend ? Friend::tmp : Friend::fileName);
        perror("");
        exit(1);
    }
    Friend buffer;
    while (inFriend >> buffer)
    {
        if (!(pseudo1 == buffer.pseudo1 && pseudo2 == buffer.pseudo2) &&
            !(pseudo1 == buffer.pseudo2 && pseudo2 == buffer.pseudo1))
            outFriend << buffer << endl;
    }
    inFriend.close();
    outFriend.close();
    rename(Friend::tmp, Friend::fileName);
    return "Done";
}

string BaseDeDonnee::demandeFriend(string pseudo1, string pseudo2)
{
    if (pseudo1 == pseudo2)
        return "Vous ne pouvez pas vous envoyer une demande d'amis à vous même";
    if (!exist(pseudo2))
        return pseudo2 + " n'existe pas";
    if (areFriend(pseudo1,pseudo2)){
        return "Vous êtes déjà amis avec " + pseudo2;
    }
    ifstream inDemande{"invit.txt", ios::in};
    if (!inDemande)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", "invit.txt");
        perror("");
        exit(1);
    }
    Friend buffer;
    while (inDemande >> buffer)
    {
        if (buffer.pseudo1 == pseudo2 && buffer.pseudo2 == pseudo1)
        {
            inDemande.close();
            return pseudo2 + " vous a déjà envoyé une demande d'amis";
        }
        else if (buffer.pseudo1 == pseudo1 && buffer.pseudo2 == pseudo2)
        {
            inDemande.close();
            return "Vous avez déjà envoyé une demande d'amis à " + pseudo2;
        }
    }
    inDemande.close();
    ofstream outDemande{"invit.txt", ios::out | ios::app};
    if (!outDemande)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", "invit.txt");
        perror("");
        exit(1);
    }
    buffer.pseudo1 = pseudo1; buffer.pseudo2 = pseudo2;
    outDemande << buffer << endl;
    outDemande.close();
    return "Done";
}

string BaseDeDonnee::refuseDemande(string pseudo1, string pseudo2)
{
    if (pseudo1 == pseudo2)
        return "Vous ne pouvez pas refuser votre propre demande d'amis";
    if (!exist(pseudo2))
        return pseudo2 + " n'existe pas";
    ifstream inDemande{"invit.txt", ios::in};
    ofstream outDemande{"tmpInvit.txt", ios::out};
    if (!inDemande || !outDemande)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", inDemande ? "tmpInvit.txt" : "invit.txt");
        perror("");
        exit(1);
    }
    Friend buffer;
    while (inDemande >> buffer)
        if (!(buffer.pseudo2 == pseudo1 && buffer.pseudo1 == pseudo2))
            outDemande << buffer << endl;
    inDemande.close();
    outDemande.close();
    rename("tmpInvit.txt", "invit.txt");
    return "Done";
}

string BaseDeDonnee::getFriends(string pseudo)
{
    ifstream inFriend{Friend::fileName, ios::in};
    ostringstream ret{"", ios::out};
    if (!inFriend)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", Friend::fileName);
        perror("");
        exit(1);
    }
    Friend buffer;
    while (inFriend >> buffer)
        if (buffer.pseudo1 == pseudo)
            ret << buffer.pseudo2 << endl; 
    inFriend.close();
    return ret.str();
}

string BaseDeDonnee::getFriendsAttente(string pseudo)
{

    ifstream inDemande{"invit.txt", ios::in};
    ostringstream ret{"", ios::out};
    if (!inDemande)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", "invit.txt");
        perror("");
        exit(1);
    }
    Friend buffer;
    while (inDemande >> buffer)
        if (buffer.pseudo2 == pseudo)
            ret << buffer.pseudo1 << endl; 
    inDemande.close();
    return ret.str();
}

/**
 * @param chat doit être sous forme <message1>\\n<message2>\\n...<messagei>\\n ou vide pour supprimer complêtement le chat 
 */
void BaseDeDonnee::setChat(string pseudo1, string pseudo2, string chat)
{
    if (pseudo1 == pseudo2)
        return; // pseudo1 ne peut pas avoir de chat avec lui même
    ifstream inFile{Chat::fileName, ios::in};
    ofstream outFile{Chat::tmp, ios::out};
    if (!inFile || !outFile)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", inFile ? Chat::tmp : Chat::fileName);
        perror("");
        exit(1);
    }
    Chat buffer;
    while (inFile >> buffer)
    {
        if ((pseudo1 == buffer.pseudo1 && pseudo2 == buffer.pseudo2) ||
            (pseudo1 == buffer.pseudo2 && pseudo2 == buffer.pseudo1))
        {
            if (!chat.empty())
            {
                buffer.chat = chat;
                chat = "";
                outFile << buffer << endl;
            }
        }
        else
            outFile << buffer << endl; 
    }
    if (!chat.empty())
    {
        buffer.pseudo1 = pseudo1; buffer.pseudo2 = pseudo2; buffer.chat = chat;
        outFile << buffer << endl;
    }
    inFile.close();
    outFile.close();
    rename(Chat::tmp, Chat::fileName);
    return;
}

/**
 * @return string sous forme <message1>\\n<message2>\\n...<messagei>\\n
 */
string BaseDeDonnee::getChat(string pseudo1, string pseudo2)
{
    if (pseudo1 == pseudo2)
        return ""; // pseudo1 ne peut pas avoir de chat avec lui même
    ifstream inFile{Chat::fileName, ios::in};
    if (!inFile)
    {
        fprintf(stderr, "Impossible d'ouvrir %s", Chat::fileName);
        perror("");
        exit(1);
    }
    
    Chat buffer;
    while (inFile >> buffer)
    {
        if ((pseudo1 == buffer.pseudo1 && pseudo2 == buffer.pseudo2) ||
            (pseudo1 == buffer.pseudo2 && pseudo2 == buffer.pseudo1))
        {
            inFile.close();
            return buffer.chat;
        }
    }
    return ""; // il n'existe pas de chat entre pseudo1 et pseudo2
}

bool BaseDeDonnee::exist(const string &username)
{
    // Vérfie si le User existe dans la base de donnée.

    fstream dbuser;
    dbuser.open("db_user.txt", ios::in);
    if (dbuser.is_open())
    {
        string line;
        string cmp = username;
        while (getline(dbuser, line, ',')) // prend uniquement l'username
        {
            if (line == cmp)
            {
                dbuser.close();
                return true;
            }
            getline(dbuser, line); // pass le password
        }
        dbuser.close();
        return false;
    }
    return false;
}

bool BaseDeDonnee::check_exist(const string &username, const string &password)
{
    // Vérfie si le User et password existe dans la base de donnée.

    fstream dbuser;
    dbuser.open("db_user.txt", ios::in);
    if (dbuser.is_open())
    {
        string line;
        string cmp = username + "," + password;
        while (getline(dbuser, line))
        {
            if (line == cmp)
            {
                dbuser.close();
                return true;
            }
        }
        dbuser.close();
        return false;
    }
    return false;
}

bool BaseDeDonnee::create(const string &username, const string &password)
{
    // Crée un User dans la base de donnée si le username n'est pas déja pris.
    fstream dbuser;
    dbuser.open("db_user.txt", ios::in);
    if (dbuser.is_open())
    {
        string line;
        while (getline(dbuser, line))
        {
            string un = line.substr(0, line.find(","));
            if (un == username)
            {
                dbuser.close();
                return false;
            }
        }
        dbuser.close();
    }
    dbuser.open("db_user.txt", ios::app);
    if (dbuser.is_open())
    {
        string new_user = username + "," + password;
        dbuser << new_user << endl;
        dbuser.close();
        update(username, 0, getTime());
        return true;
    }
    return false;
}

bool BaseDeDonnee::login(const string &username, const string &password)
{
    fstream dbuser;
    dbuser.open("db_user.txt", ios::in);
    if (dbuser.is_open())
    {
        string line;
        string userpass = username + "," + password;
        while (getline(dbuser, line))
        {
            cout << line << " " << userpass << endl;
            if (line == userpass)
            {
                dbuser.close();
                return true;
            }
        }
        dbuser.close();
    }
    else
        cerr << "cannot open the data base" << endl;
    return false;
}


ostream &operator<<(ostream &os, BaseDeDonnee::Classement &clss)
{
    os << clss.pseudo << ' ' << clss.score << ' ' << clss.timestamp;
    return os;
}

istream &operator>>(istream &is, BaseDeDonnee::Classement &clss)
{
    is >> clss.pseudo >> clss.score >> clss.timestamp;
    return is;
}

ostream& operator<<(ostream &os, BaseDeDonnee::Friend &frd)
{
    os << frd.pseudo1 << ' ' << frd.pseudo2;
    return os;
}

istream& operator>>(istream &is, BaseDeDonnee::Friend &frd)
{
    is >> frd.pseudo1 >> frd.pseudo2;
    return is;
}

ostream& operator<<(ostream &os, BaseDeDonnee::Chat &chat)
{
    replace(chat.chat , '\n', '\x1f');
    os << chat.pseudo1 << ' ' << chat.pseudo2 << ' ' << chat.chat;
    replace(chat.chat , '\x1f', '\n' );
    return os;
}

istream& operator>>(istream &is, BaseDeDonnee::Chat &chat)
{
    is >> chat.pseudo1 >> chat.pseudo2;
    is.get();
    getline(is, chat.chat);
    replace(chat.chat, '\x1f', '\n');
    return is;
}
