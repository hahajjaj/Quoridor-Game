#include "User.hpp"
void User::closeFd()
{
    close(fd);
    fd = -1;
}

void User::reset()
{
    status.clear();
    username.clear();
    password.clear();
    chattingWith.clear();
    partie = nullptr;
    tournoi = nullptr;
}

bool User::hasSendInviteTo(string pseudo)
{
    for (auto &invit : invitations)
    {
        if (invit.pseudo == pseudo)
            return true;
    }
    return false;
}

void User::removeAllInvitationGame()
{
    invitations.clear();
}

void User::setTournoi(Serveur* otherTournoi){ tournoi = make_shared<Tournoi>(otherTournoi); }

string User::showInvitation()
{
    if (invitations.empty()){
        return "Pas d'invitations :(";
    }
    string inviteur = "";
    for (auto inv : invitations){
        inviteur += inv.pseudo;
        switch (inv.type)
        {
        case TOURNOIS:
            inviteur += " --Tournois";
            break;
        case REPRENDREPARTIE:
            inviteur += " --reprise de partie";
            break;
        default:
            break;
        }
        inviteur += '\n'; 
    }
    return inviteur;
}

void User::sendInvitationGame(string pseudo, invitationType type)
{
    if (!hasSendInviteTo(pseudo))
        invitations.push_back({pseudo, type});
}

void User::removeInvitationGame(string pseudo,  invitationType type)
{
    for( std::vector<invit>::iterator iter = invitations.begin(); iter != invitations.end(); ++iter )
    {    
        cout << iter->pseudo << pseudo << (pseudo == iter->pseudo) << endl;
        if (pseudo == iter->pseudo){
	        invitations.erase(iter);
            return;
        }
    }
}


void User::sendSauvegardeGame(string pseudo1, string pseudo2, string etat_plateau)
{
    string invit = pseudo1 + "," + pseudo2;
    removeSauvegardeGame(invit);
    fstream db_partie;
    db_partie.open("sauvegardePartie.txt", ios::app);

    if (db_partie.is_open())
    {
        invit = "2," + invit + "," + etat_plateau;
        db_partie << invit << endl;
        db_partie.close();
    }
}

void User::sendSauvegardeGame(vector<string> pseudos, string etat_plateau)
{
    string res = "";
    res += to_string(pseudos.size());
    res += ",";
    for (long unsigned int i = 0; i < pseudos.size(); i++)
    {
        res += pseudos[i] + ",";
    }
    removeSauvegardeGame(res.substr(2, res.size() - 2 - 1));
    res += etat_plateau;
    fstream db_partie;
    db_partie.open("sauvegardePartie.txt", ios::app);
    if (db_partie.is_open())
    {
        db_partie << res << endl;
        db_partie.close();
    }
}

void User::sendSauvegardeGame(string pseudo1, string pseudo2, string pseudo3, string pseudo4, string etat_plateau)
{
    string invit = pseudo1 + "," + pseudo2 + "," + pseudo3 + "," + pseudo4;
    removeSauvegardeGame(invit);
    fstream db_partie;
    db_partie.open("sauvegardePartie.txt", ios::app);

    if (db_partie.is_open())
    {
        invit = "4," + invit + "," + etat_plateau;
        db_partie << invit << endl;
        db_partie.close();
    }
}

string User::removeSauvegardeGame(string liste_pseudo)
{
    string to_return = "";
    string line;
    ifstream fin;

    fin.open("sauvegardePartie.txt");
    ofstream temp;
    temp.open("temp.txt");

    while (getline(fin, line))
    {
        if (line.find(liste_pseudo) == string::npos)
            temp << line << endl;
        else
            to_return = line.substr(line.find(liste_pseudo) + liste_pseudo.size() + 1);
    }

    temp.close();
    fin.close();
    remove("sauvegardePartie.txt");
    rename("temp.txt", "sauvegardePartie.txt");
    return to_return;
}

string User::getSauvegardePartie(string pseudo)
{
    ifstream db_partie("sauvegardePartie.txt");
    string to_return = "";

    if (db_partie.is_open())
    {
        string line;
        while (getline(db_partie, line))
        {
            string tmp = "";
            istringstream mot{line};

            getline(mot, line, ',');
            int nombre_joueurs = stoi(line);
            bool inSave = false;
            for (int i = 0; i < nombre_joueurs; i++)
            {
                getline(mot, line, ',');
                if (line == pseudo)
                    inSave = true;
                tmp += line + (i < nombre_joueurs -1 ? "," : "");
            }
            if (inSave)
                to_return += tmp + "\n";
        }
        db_partie.close();
    }
    if (!to_return.empty())
        to_return.pop_back(); // \n
    return to_return;
}

int User::taille(string str)
{
    int length = 0;
    for (uint i = 0; str[i] != '\0'; i++)
    {
        length++;
    }
    return length;
}

vector<string> User::separateur(string str, char seperator)
{
    int i = 0, startIndex = 0, endIndex = 0;
    vector<string> vect;
    while (static_cast<int>(i) <= taille(str))
    {
        if (str[i] == seperator || static_cast<int>(i) == taille(str))
        {
            endIndex = i;
            string subStr = "";
            subStr.append(str, startIndex, endIndex - startIndex);
            vect.push_back(subStr);
            startIndex = endIndex + 1;
        }
        i++;
    }
    return vect;
}

void User::login(string n, string p){
    cout << "Connected\n";
    setUsername(n);
    setPassword(p);
    setLogged();
}