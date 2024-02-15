
#include "Serveur.hpp"

void Serveur::run(int port)
{
	int activity = 1;
	fd_set readfds; // fd_set = nombre maximum de fd dont le fd_set peut gérer les informations.
					// la liste des file descriptor que le select utilise.
	int opt = 1;
	struct sockaddr_in address;
	size_t addrlen;

	serveur_fd = socket(AF_INET, SOCK_STREAM, 0);                                       // Création d'un socket --> protocole de communication sur le socket: TCP.
	setsockopt(serveur_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)); // Mise en place des options (réutilisation du port et de l'adresse).
	addrlen = sizeof(address);
	memset(&address, '0', addrlen);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY); // Liaison du file descriptor à toutes les interfaces disponibles sur la machine.
	address.sin_port = htons(port);
	if (::bind(serveur_fd, (struct sockaddr *)&address, addrlen) == -1) // Liaison entre le file descriptor et le port contenu dans la structure d'adresse.
		return;
	listen(serveur_fd, 3);                                                                // Ecoute les demandes de connexion sur le socket. Maximum 3 connexions en attente d'acceptation.
	int max_fd = set_fd_sets(readfds);
	while (activity > 0)
	{
		max_fd = set_fd_sets(readfds);
		activity = select(max_fd + 1, &readfds, nullptr, nullptr, nullptr); // Attente bloquante des fd
		if (activity > 0)
		{
			if (FD_ISSET(serveur_fd, &readfds)) // Verifie si le socket du serveur est en activité.
				newClient();
			else
				messageReceive(readfds);
		}
	}
	close(serveur_fd);
}

void Serveur::newClient(){
	int new_fd = accept(serveur_fd, nullptr, nullptr); // Recupere le file descriptor qui permet de communiquer avec le client.
	cout << "Nouveaux client: " << new_fd;
	users.push_back(new User{new_fd});
	string buff = "";
	recvMessage(new_fd, buff);
	if (buff == "gui")
	{
		users.back()->setGui(true);
	}
	else
		sendTo(users.back(), "3" + LOGIN);
}

int Serveur::recvMessage(int sock, string &dest, bool block)
{
    timeval timeOut{0, 0};
	fd_set readFd;
    FD_ZERO(&readFd);
    FD_SET(sock, &readFd);

    if (block || (checked(select(sock + 1, &readFd, nullptr, nullptr, &timeOut)) && FD_ISSET(sock, &readFd)))
    {
        char *buffer;
        size_t recv = receive(sock, (void**)&buffer, nullptr);
        if (recv == 0) // connection lost
            return -1;
        dest = buffer;
        delete buffer;
        return recv;
    }
    return 0;
}

void Serveur::removeAllInvitGame(User *u)
{
	if (u->isLogged())
	{
		for (auto &user : users)
		{
			if (user != u)
				user->removeInvitationGame(u->getUsername());
		}
		// u->removeAllInvitationGame();
	}
}

Serveur::~Serveur()
{
	for (auto &u : users)
	{
		u->closeFd();
		delete u;
	}
}

void Serveur::sendTo(User *u, const string &m)
{
	ssend(u->getFd(), m.c_str(), m.size() + 1);
}

void debug(char *message, User *u)
{
	cout << "message : " << message << " u->status : " << u->getStatus() << endl;
}

void Serveur::disconnectCLient(User *u){
	cerr << "client disconected" << endl;
	
	// TODO is in Game => forfait
	u->closeFd();
	removeAllInvitGame(u);
	delete u;
	users.erase(std::find(users.begin(), users.end(), u));
}

int Serveur::set_fd_sets(fd_set &readfds)
{
	FD_ZERO(&readfds);            // Initialise le fd_set à 0 bits pour tous les file descriptors.
	FD_SET(serveur_fd, &readfds); // Met les bits pour serveurFd dans le fd_set &readfds.
	int max_fd = serveur_fd;

	for (auto u : users)
	{
		// Met les bits pour le fd de chaque client dans &readfds.
		int fd = u->getFd();
		if (fd > 0)
			FD_SET(fd, &readfds);
		if (fd > max_fd) // Récupère le fd dont la valeur est la + grande
			max_fd = fd;
	}
	return max_fd;
}


bool Serveur::messageReceive(fd_set &readfds)
{
	char *rec;
	for (auto u : users)
	{
		if (FD_ISSET(u->getFd(), &readfds))
		{
			if (receive(u->getFd(), (void **)&rec, nullptr) == 0)
			{
				disconnectCLient(u);
				return false;
			}
			else
			{
				if (u->isGui())
					parseGui(rec, u);
				delete rec;
			}
		}
	}
	return true;
}

/**
 * format:
 * 	"<>" => obligatoire
 * 	"[]" => optionnel
 * 
 * 
 *	Fonction Serveur		| Message du client											| Réponse du serveur
 * -Login:					|															|
 * 	-login 					| "l<pseudo>\n<password>"									| "1" if succes else "0[message erreur]"
 * 	-register				| "r<pseudo>\n<password>"									| "1" if succes else "0[message erreur]"
 * 	-deconnexion			| "d"														| "1" if succes else "0[message erreur]"
 * 							|															|
 * -Game:					|															|
 * 	-gameMove				| "g<move>"													| "1<status plateau>" if succes else "0[message erreur]" if succes else "0[message erreur]" 
 * 	-gamePause				| "gpause"													| "1" if succes else "0[message erreur]"
 * 	-gameCreatePartie		| "gc"														| "1" if succes else "0[message erreur]"
 * 	-gameDeletePartie		| "gd"														| "1" if succes else "0[message erreur]"
 * 	-gameForfaitPartie		| "gf"														| "1" if succes else "0[message erreur]"
 * 	-gameLancerPartie		| "gl<mode partie (Normal/Ttem/Tournoi)>\n<nb joueurs(2/4)>"| "1" if succes else "0[message erreur]" then if succes "1<status plateau>" if partie not over else "2<message fin partie/partie sauvegardé>"
 * 	-gameReprendrePartie	| "gr<pseudo1>,<pseudo2>,<...>"								| "1" if succes else "0[message erreur]" then if succes "1<status plateau>" if partie not over else "2<message fin partie/partie sauvegardé>"
 * 	-gameJoinPartie			| "gj<pseudo du joueur qui a envoyé l'invite>"				| "1" if succes else "0[message erreur]"
 * 	-gameSendInvite			| "gs<pseudo du joueur à invité>"							| "1" if succes else "0[message erreur]" 
 * 	-gameGetInvite			| "ggi"														| "1<pseudo1>\n<pseudo2>\n<...>" if succes else "0[message erreur]"
 * 	-gameGetSave			| "ggs"														| "1<liste joueurs de la partie1>\n<liste joueurs de la partie2>\n<...>" if succes else "0[message erreur]"
 * 							|															|
 * -Data Base				|															|
 * 	-getClassementGlobal	| "cg"														| "1<pseudo rank 1> <score rank 1> <timestamp rank 1>\n<pseudo rank 2> <...>" if succes else "0[message erreur]"
 * 	-getClassementFriend	| "cf"														| "1<pseudo rank 1> <score rank 1> <timestamp rank 1>\n<pseudo rank 2> <...>" if succes else "0[message erreur]"
 * 	-getClassementHebdo		| "ch"														| "1<pseudo rank 1> <score rank 1> <timestamp rank 1>\n<pseudo rank 2> <...>" if succes else "0[message erreur]"
 * 							|															|
 * -Friends					|															|
 * 	-friendInvite 			| "fi<pseudo qui recoit la demande d'amis>"					| "1" if succes else "0[message erreur]" if succes else "0[message erreur]"
 * 	-friendAccepte 			| "fa<pseudo de celui qui a envoyé la demande d'amis>"		| "1" if succes else "0[message erreur]" if succes else "0[message erreur]"
 * 	-friendRefuse			| "fr<pseudo de celui qui a envoyé la demande d'amis>"		| "1" if succes else "0[message erreur]" if succes else "0[message erreur]"
 * 	-friendDel				| "fd<pseudo qui doit être supprimé de la de ses amis>"		| "1" if succes else "0[message erreur]" if succes else "0[message erreur]"
 * 	-friendGetFriends		| "fgf"														| "1<pseudo friend 1>\n<pseudo friend 2>\n<...>" if succes else "0[message erreur]"
 * 	-friendGetInvite 		| "fgi"														| "1<pseudo invite 1>\n<pseudo invite 2>\n<...>" if succes else "0[message erreur]"
 * 	-friendGetConnected		| "fgc['o']" (si 'o' -> offline to)							| "1<pseudo friend>\n<...>" if succes else "0[message erreur]"
 * 	-friendIsConnected		| "fv<pseudo d'un amis>"									| "1" if est amis et connecté else "0[message erreur]"
 * 							|															|
 * -Messagerie				|															|
 * 	-messagerieGet			| "mg[pseudo avec qui on discute]" (set/reset chatWith)		| "1[<pseudo envoyeur>: <message>\n<pseudo envoyeur>: <message>\n<..>]" if succes else "0[message erreur]" then if succes "1<chatLog>\n<nouveau message>"
 * 	-messagerieSend			| "ms<message>"												| "1<pseudo envoyeur>: <message>\n<pseudo envoyeur>: <message>\n<..>" if succes else "0[message erreur]"
 * 							|															|
 * 	-UNKNOWN				| "<UNKNOWN>"												| "0" default
 *
 * @param message 
 * @param u 
 */

void Serveur::parseGui(string message, User *u)
{
	cout << "parseGui" << endl;
	if (message.empty())
	{
		sendTo(u, "0"); // UNKNOWN
	}
	else if (! u->isLogged())
	{
		parseLogin(message, u);
	}
	else
	{
		switch (message[0])
		{
		case 'g': // Game
			parseJoue(message, u);
			break;
		case 'c': // Classement
			parseClass(message, u);
			break;
		case 'f': // Friends
			parseAmis(message, u);
			break;
		case 'm': // messagerie/chat
			parseChat(message, u);
			break;
		case 'd': // deconnexion
			removeAllInvitGame(u);
			u->reset();
			u->setLogged();
			sendTo(u, "1");
			break;
		default: // UNKNOWN
			break;
		}
	}
}

void Serveur::parseAmis(string message, User *u){
	string response = "";
	if (message.size() < 2) // UNKNOWN
	{
		sendTo(u, "0");
		return;
	}
	if (message.size() >= 3){ // UNKNOWN
		switch(message[1])
		{
			case 'i':	// friendInvite
				response = db->demandeFriend(u->getUsername(), message.substr(2));
				if (response != "Done")
					break;
				sendTo(u, "1" + response);
				return;
			case 'a':	// friendAccepte
				response = db->addFriend(u->getUsername(), message.substr(2));
				if (response != "Done")
					break;
				sendTo(u, "1" + response);
				return;
			case 'r':	// friendRefuse
				response = db->refuseDemande(u->getUsername(), message.substr(2));
				if (response != "Done")
					break;
				sendTo(u, "1" + response);
				return;
			case 'd':	// friendDel
				response = db->delFriend(u->getUsername(), message.substr(2));
				if (response != "Done")
					break;
				sendTo(u, "1" + response);
				return;
			case 'g':
				switch(message[2])
				{
					case 'f':	// friendGetFriends
						sendTo(u, "1" + db->getFriends(u->getUsername()));
						return;
					case 'i':	// friendGetInvite
						sendTo(u, "1" + db->getFriendsAttente(u->getUsername()));
						return;
					case 'c':
						if (message.size() > 3) // offline
							response = getConnectedFriends(u);
						else
							response = getConnectedFriends(u, false);
						sendTo(u, "1" + response);
						return;
					default: // UNKNOWN
						break;
				}
				break;
			case 'v': // friendIsConnected
			{
				string ami = message.substr(2);
				if (!db->exist(ami))
				{
					response = "C'est utilisateur n'existe pas (" + ami + ")";
					break;
				}
				if (!db->areFriend(u->getUsername(), ami))
				{
					response = "Vous n'êtes pas amis avec " + ami;
					break;
				}
				if (!findUser(ami.c_str()))
				{
					response = ami + " n'est pas connecté";
					break;
				}
				sendTo(u, "1");
				return;
			}
			default: // UNKNOWN
				break;
		}
	}
	sendTo(u, "0" + response);
	return;
}

void Serveur::parseLogin(string message, User *u)
{
	string response = "";
	u->reset();

	size_t sepIndex = message.find("\n");
	string pseudo = message.substr(1, sepIndex - 1);
	string password = message.substr(sepIndex + 1);
	if (sepIndex == string::npos)
	{
		sendTo(u, "0");
		return; // UNKNOWN
	}
	switch (message[0])
	{
	case 'l': // login
	{
		if (findUser(pseudo.c_str())){
			response = "Vous etes deja connecte avec un autre client";
			break;
		}
		else if (!db->login(pseudo, password))
		{
			response = "Mauvais pseudo ou mot de passe. Ressayez";
			break;
		}
		else
		{
			u->login(pseudo, password);
			sendTo(u, "1");
			return;
		}
		break;
	}
	case 'r': // register
	{
		if (pseudo.find(' ') != string::npos || password.find(' ') != string::npos)
		{
			response = "Il ne peut pas y avoir d'espace dans le pseudo ou le mot de passe. Ressayez";
			u->reset();
			break;
		}
		else if (!db->create(pseudo, password))
		{
			response = "Impossible de creer un nouveau compte pour l'instant. Ressayez dans un petit moment";
			u->reset();
			break;
		}
		else
		{
			u->login(pseudo, password);
			sendTo(u, "1");
			return;
		}
		
	}
	default: // UNKNOWN or not logged in yet
		break;
	}
	sendTo(u, "0" + response);
}

void Serveur::parseChat(string message, User *u)
{
	string response = "";
	if (message.size() >= 2){ // UNKNOWN
		switch(message[1])
		{
			case 'g':
				u->setChattingWith(message.substr(2));
				if (!u->getChattingWith().empty())
				{
					if (!db->exist(u->getChattingWith()))
					{
						response = "Ce pseudo n'existe pas";
						break;
					}
					response =  db->getChat(u->getUsername(), message.substr(2));
				}
				sendTo(u, "1" + response);
				return;
			case 's':
			{
				if (message.size() < 3) // UNKNOWN
				{
					response = "Le message envoyé est vide";
					break;
				}
				else if (u->getChattingWith().empty())
				{
					response = "First mg<pseudo avec qui discuté>";
					break;
				}
				string msg = message.substr(2);
				string chatBuff = db->getChat(u->getUsername(), u->getChattingWith());
				chatBuff.append(u->getUsername() + ": " + msg + "\n");
				db->setChat(u->getUsername(), u->getChattingWith(), chatBuff);
				User *destU = findUser(u->getChattingWith().c_str());
				if (destU && destU->getChattingWith() == u->getUsername())
					sendTo(destU, "1" + chatBuff);
				sendTo(u, "1" + chatBuff);
				return;
			}
			default: // UNKNOWN
				break;
		}
	}
	sendTo(u, "0" + response);
}

void Serveur::parseClass(string message, User *u)
{
	if (message.size() >= 2){
		switch(message[1])
		{
			case 'g':	// getClassementGlobal
				sendTo(u, "1" + db->getGlobalClass());
				break ;
			case 'f':	// getClassementFriend
				sendTo(u, "1" + db->getFriendsClass(u->getUsername()));
				break;
			case 'h':	// getClassementHebdo
				sendTo(u, "1" + db->getHebdoClass());
				break;
			default: // UNKNOWN
				sendTo(u, "0");
				break;
		}
	}
	else // UNKNOWN
		sendTo(u, "0");
}

void Serveur::parseJoue(string message, User *u)
{
	string response = "";
	if (message.size() < 2) // UNKNOWN
		sendTo(u, "0");
	else{
		switch(message[1])
		{
			case 'm': // gameMove
				parseMovePartie(message, u);
				break;
			case 'p': // gamePause
				savePartie(message, u);
				break;
			case 'c': // gameCreatePartie
				sendTo(u, "1");
				break;
			case 'd': // gameDeletePartie
				gameDeletePartie(message, u);
				break;
			case 'f': // gameForfaitPartie
				gameForfaitPartie(message, u);
				break;
			case 'l': // gameLancePartie	
				gameLancePartie(message, u);
				break;
			case 'r': // gameReprendrePartie
				gameReprendrePartie(message, u);
				break;
			case 'j': // gameJoinPartie
				parseJoinPartie(message, u);
				break;
			case 's': // gameSendInvite
				gameSendInvite(message, u);
				break;
			case 'g':
				if (message.size() < 3) // UNKNOWN
					sendTo(u, "0");
				switch(message[2])
				{
					case 'i': // gameGetInvite
						sendTo(u, "1" + u->showInvitation());
						return;
					case 's': // gameGetSave
					{
						string pseudos = u->getSauvegardePartie(u->getUsername());
						if (pseudos.empty())
						{
							sendTo(u, "0Pas de partie sauvegardée");
							return;
						}
						sendTo(u, "1" + pseudos);
						return;
					}
					default:
						sendTo(u, "0");
						return;
				}
				break;
			default: // UNKNOWN
				sendTo(u, "0");
		}
	}
}

void Serveur::parseMovePartie(string message, User *u)
{
	string response = "";
	if (message.size() < 3)
	{
		sendTo(u, "0Erreur format du coup");
	}
	else if (!u->isInGame() && !u->inTournoi())
	{
		sendTo(u, "0Vous n'êtes pas dans une partie");
	}
	else{
		if (u == u->getPartie()->getUser() && !u->getPartie()->isFinished())
		{
			char *coup = strdup(message.substr(2).c_str());
			u->getPartie()->jouerCoup(coup);
			delete coup;
		}
		if (u->inTournoi() && u->getPartie()->isFinished())
		{
			u->getTournoi()->newRound();
		}
		else if (u->getPartie()->isFinished())
		{
			cout << "La partie est finie " << endl;
			int joueur_gagnant = u->getPartie()->joueur_gagnant;
			printf("joueur gagnant %d\n", joueur_gagnant);
			db->update(u->getPartie()->users[joueur_gagnant]->getUsername(), db->getScore(u->getPartie()->users[joueur_gagnant]->getUsername()) + 1);

			Partie *partie_to_remove = u->getPartie();
			int len = u->getPartie()->users.size();
			for (int i = 0; i < len; i++)
			{
				User *user = partie_to_remove->users[i]; 
				user->setPartie(nullptr);
				if (i == joueur_gagnant)
					sendTo(user, "2Vous avez gagné la partie !!!");
				else
					sendTo(user, "2Vous avez perdu la partie :(\nGagnant: " + partie_to_remove->users[joueur_gagnant]->getUsername());
			}
			delete partie_to_remove;
		}
	}
}

void Serveur::gameDeletePartie(string message, User *u){
	Partie *partie = u->getPartie();
	if (!partie)
	{
		sendTo(u, "0Vous n'avez pas encore créé de partie.");
		return;
	}
	int len = partie->users.size();
	for (int i = 0; i < len; ++i)
	{
		User *user = partie->users[i];
		user->setPartie(nullptr);
		if (user != u)
			sendTo(user, "3La partie a été annulé par " + u->getUsername());
	}
	delete partie;
	u->removeAllInvitationGame();
	sendTo(u, "1");
	return;
}

void Serveur::gameForfaitPartie(string message, User *u){
	if (!u->isInGame())
	{
		sendTo(u, "0Vous n'êtes pas dans une partie.");
		return;
	}
	if (u->inTournoi())
	{
		sendTo(u, "0Vous ne pouvez pas déclaré forfait en tournoi.");
		return;
	}
	Partie *partie = u->getPartie();
	if (!partie->joueurPret())
	{
		sendTo(u, "0La partie n'a pas encore commencé.");
		return;
	}
	int len = partie->users.size();

	int oldScore = db->getScore(u->getUsername());
	db->update(u->getUsername(), oldScore <= 0 ? 0 : oldScore - 1);
		
	for (int i = 0; i < len; ++i)
	{
		User *user = partie->users[i];
		user->setPartie(nullptr);
		if (user != u)
			sendTo(user, "2" + u->getUsername() + " a declare forfait.");
	}
	delete partie;
	u->removeAllInvitationGame();
	
	sendTo(u, "2Vous avez déclaré forfait.");
	return;
}

void Serveur::savePartie(string message, User *u){
	if (u->getPartie()->ModeJeu != "Normal")
	{
		sendTo(u, "0Impossible de sauvegarder une partie en mode " + u->getPartie()->ModeJeu);
		return;
	}
	// Sauvegarder l'état du plateau dans la bdd
	cout << "Pause" << endl;

	// Quitter le mode isInGame et supprimer l'instance de Partie
	Partie *partie_to_remove = u->getPartie();
	int len = u->getPartie()->users.size();
	vector<string> pseudos;
	for (int i = 0; i < len; i++)
	{
		pseudos.push_back(u->getPartie()->users[i]->getUsername());
	}
	// sauvegarde les joueurs dans le fichier txt pour la reprise de partie

	u->sendSauvegardeGame(pseudos, u->getPartie()->jeu->parseMessage());

	for (int i = 0; i < len; i++)
	{
		User *user = partie_to_remove->users[i];
		user->setPartie(nullptr);
		sendTo(user, "2La partie a été sauvegardé");
	}
	delete partie_to_remove;
	return;
}

void Serveur::gameReprendrePartie(string message, User *u){
	if (message.size() < 3)
	{
		sendTo(u, "0Error format. Pseudo séparé d'une virgule sans espace");
		return;
	}
	string response = reprendrePartieSauvegarde(message.substr(2), u);
	if (response != "Done")
		sendTo(u, "0"+response);
	sendTo(u, "1");
	u->getPartie()->setReady();
	u->getPartie()->addUser(u);
	u->getPartie()->play(u->getPartie()->etat_plateau);
	cout << "etat plateau: "  << u->getPartie()->etat_plateau << endl;
	return;
}

void Serveur::parseJoinPartie(string message, User *u)
{
	if (message.size() < 3) // UNKNOWN
		sendTo(u, "0");
	else{
		string invite = message.substr(2);
		User *joueur = findUser(invite.c_str());

		if (!joueur)
		{
			sendTo(u, "0" + format("Ce joueur n'est pas connecté ou n'existe pas (%s)", invite.c_str()));
			return;
		}

		u->removeInvitationGame(joueur->getUsername());

		if (joueur->inTournoi())
		{
			sendTo(u, "1");
			u->setTournoi(joueur->getTournoi());
			u->getTournoi()->addPlayer(u);
		}
		else
		{
			if (joueur->getPartie()->joueurPret())
			{
				sendTo(u, "0La partie est full.");
				return;
			}
			sendTo(u, "1");
			joueur->getPartie()->addUser(u);
			u->setPartie(joueur->getPartie());

			if (u->getPartie()->FromSauvegarde)
				u->getPartie()->play(u->getPartie()->etat_plateau);
			else
				u->getPartie()->play();
		}
	}
}

void Serveur::gameLancePartie(string message, User *u){
	if (message.size() < 3) // UNKNOWN
		sendTo(u, "0");
	size_t sepIndex = message.find('\n');
	if (sepIndex == string::npos)
	{
		sendTo(u, "0Erreur format message (\"gl<mode de jeu (Normal/Item/Tournoi)>\\n<nb joueurs (2 ou 4)>\")");
		return;
	}
	string modeDeJeu = message.substr(2, sepIndex - 2);
	int nbJoueur = stoi(message.substr(sepIndex + 1));
	if (modeDeJeu != "Normal" && modeDeJeu != "Item" && modeDeJeu != "Tournoi")
	{
		sendTo(u, "0" + format("Erreur format message: mode de jeu inconnu (%s)", modeDeJeu.c_str()));
		return;
	}
	else if (nbJoueur != 2 && nbJoueur != 4)
	{
		sendTo(u, "0" + format("Erreur format message: nb joueurs inconnu (%d)", nbJoueur));
		return;
	}
	
	u->clearTournoi();
	if (modeDeJeu == "Tournoi")
	{
		sendTo(u, "1");
		u->setTournoi(this);
		u->getTournoi()->addPlayer(u);
	}
	else
	{
		if (u->isInGame())
		{
			sendTo(u, "0Vous êtes déjà dans une partie");
			return;
		}
		sendTo(u, "1");
		Partie *partie = new Partie(u, this);
		partie->setReady();
		partie->ModeJeu = modeDeJeu;
		partie->setNbJoueur(nbJoueur);
		partie->addUser(u);
		partie->play();
	}
	return;
}

void Serveur::gameSendInvite(string message, User *u){
	if (message.size() < 3) {// UNKNOWN
		sendTo(u, "0");
		return;
	}
	string invite = message.substr(2);
	if (!db->areFriend(u->getUsername(), invite)){
		sendTo(u, "0" + format("Vous n'êtes pas amis avec %s", invite.c_str()));
		return;
	}
	User *i = findUser(invite.c_str());
	if (!i)
	{
		sendTo(u, "0" + invite + " n'est pas connecté.");
		return;
	}
	else if (i->hasSendInviteTo(u->getUsername())){
		sendTo(u, "0" + format("Vous avez déjà envoyé une invitation à %s", invite.c_str()));
		return;
	}
	i->sendInvitationGame(u->getUsername());
	sendTo(u, "1");
	return;
}

User *Serveur::findUser(const char *username)
{
	for (auto &user : users)
	{
		if (user->getUsername() == username)
		{
			return user;
		}
	}
	return nullptr;
}

string Serveur::getConnectedFriends(User *u, bool offline)
{
	if (offline)
	{
		vector<string> vec = split(db->getFriends(u->getUsername()), '\n');
		vec.pop_back();
		for (auto &str : vec)
		{
			if (findUser(str.c_str()))
				str += " (online)";
			else
				str += " (offline)";
		}
		if (vec.empty())
			return "Vous n'avez pas encore d'amis";
		return vectToStr(vec);
	}
	string friends = "";
	for (auto &user : users)
	{
		if (u != user && db->areFriend(u->getUsername(), user->getUsername()))
		{
			friends += user->getUsername() + "\n";
		}
	}

	if (friends == "")
	{
		return "Aucun de vos amis n'est connecté.";
	}
	return friends + "\nEntrez le pseudo:";
}

string Serveur::reprendrePartieSauvegarde(string message, User *u)
{
	// recois pseudo1,pseudo2 -> il faut parser et envoyer une invitation a tous les utilisateurs
	string delimiter = ",";
	string message_str(message);
	message_str += ",";

	vector<string> pseudos;
	size_t pos = 0;
	string pseudo;
	string etat_plateau = u->removeSauvegardeGame(message_str);
	if (etat_plateau.empty())
		return "Cette sauvegarde n'existe pas";//\x1f\n" + MENU_JOUE);

	while ((pos = message_str.find(delimiter)) != string::npos)
	{
		pseudo = message_str.substr(0, pos);

		pseudos.push_back(pseudo);
		message_str.erase(0, pos + delimiter.length());
	}

	// Puis cree une instance de partie (etat du plateau) et mettre en attente

	Partie *partie = new Partie(u, this, pseudos.size());
	u->setPartie(partie);

	// invite les joueurs à rejoindre
	for (long unsigned int i = 0; i < pseudos.size(); i++)
	{
		if (pseudos[i] != u->getUsername())
		{
			User *joueur = findUser(pseudos[i].c_str());
			if (!joueur)
				return pseudos[i] + " n'est pas connnecter";
			joueur->sendInvitationGame(u->getUsername());
		}
	}

	// supprimer la ligne dans le fichier

	partie->FromSauvegarde = true;
	partie->etat_plateau = etat_plateau;
	return "Done";
}


Serveur::Serveur()
{
	db = BaseDeDonnee::getInstance();
}

#include "modedejeu/JeuItem/header/JeuItem.hpp"
