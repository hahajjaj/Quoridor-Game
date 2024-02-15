## Personnes ayant contribué au projet (phase 3)
- Hajjaj Hamza, Mondesir Stan, Piraux Lionel, Ehlalouch Safouan, Ayoub Afif. (5 étudiants sur 9)


## Lancement des exécutables
Il faut utiliser la commande make sur le terminal pour compiler tous les fichiers.
Serveur : 

./serveur \<port\>
 
Client : 

./client \<address ip\> \<port\> \<terminal/gui\>
 
terminal = interface principal fonctionelle (par défaut).
terminal2 = ancienne interface contenant certains bugs d'affichage liés à libraire ncurses.

 
## Taille du terminal 
 
Celui-ci doit être suffisament grand pour afficher correctement le menu ainsi que le plateau de jeu. La modification de la taille du terminal lorsque l'application est lancée peut causer des problèmes d'affichages. Il faut donc veiller à ouvrir un terminal assez grand avant de lancer l'application.

 
## Mode d'emploi du jeu

Pour jouer, il faut que le joueur adverse soit dans la liste d'ami. Il faut que l'utilisateur se rende dans "Jouer", "Créer une partie", et qu'il configure les options de sa partie (c'est à dire le mode de jeu, le nombre de joueurs). Il doit ensuite inviter un joueur (s'il s'agit d'une partie à 2 joueurs) ou 3 joueurs (dans le cas d'une partie à 4 joueurs). Et enfin, il doit appuyer sur "Lancer la partie". Il sera mis en attente tant que le joueurs adverse n'a pas rejoins. 
 
Une fois l'invitation envoyé, l'adversaire pourra la trouver dans la section "Voir les invitations à jouer".
Pour rejoindre la partie, il faut taper le pseudo du joueur qui invite. Et enfin la partie peut commencer. 

Il y a 3 modes de jeu possible:
 - Jouer une partie normale en 1 vs 1 (partie à deux joueurs) ou 1 vs 1 vs 1 vs 1 (partie à 4 joueurs).
 - Jouer une partie en mode Item (deux items disponible, suppression de mur avec la touche s, et saut de mur)
 - Jouer en mode tournoi

Utilisez le curseur pour deplacer un pion
 - Utilisez les flèches pour deplacer le curseur
 - Utilisez la touche Enter pour valider un choix


Pour passer le curseur en mode mur, utilisez la touche m. Une fois fait, vous pourrez utiliser la rotation de mur a l'aide de la touche r.

Une partie peut être mise en pause à l'aide de la touche x. Celle ci sera sauvegardé et pourra être récupérer ultérieurement via la l'option "reprise d'une partie sauvegardée. Ces joueurs seront ensuite invités à rejoindre la partie sauvegardée et doivent la rejoindre via l'option "Voir mes invitations à jouer". La partie sera lancée une fois que tous les joueurs auront accepter l'invitation. 
 
 ## Tournois
 
 Pour lancer le mode de jeu tournoi, il faut le selectionner lors de la création d'une partie dans la section "mode de jeu". Et ensuite il doit inviter tous les joueurs du tournoi et pour finir lancer ca partie.
 
 En mode tournoi les joueurs invités par l'organisateur du tournoi sont placer dans des partie 1 vs 1. 
 Si le nombre de joueurs est impair, le joueur en plus est considéré comme vainqueur par défaut.
 Quand toutes les parties sont fini un nouveau round commence avec les vainqueurs du dernier round.

 ## Item
Lors du mode de jeu Item, deux items sont disponibles. Un offrant la possibilité de supprimer un mur en appuyant sur la touche "s" du clavier. Et l'autre permettant de traverser un seul mur. Les items sont utilisable qu'une seule fois, une fois consommé le pouvoir attribué par l'item n'est plus actif. 
 
 . 
 ## Pause et reprise d'une partie sauvegardée 
 En partie, pour effectuer une pause, le joueur doit appuyer sur la touche "x". La partie est alors terminée et sauvegardée. Les deux joueurs peuvent la reprendre, pour cela, un joueur doit aller dans l'option "reprendre une partie sauvegardée" et il doit choisir la partie sauvegardée à reprendre. Il selectionne à l'aide des flèches la partie qu'il veut reprendre.  Les joueurs concernés recevront une demande d'invitation et doivent se rendre dans la section "Voir mes invitations à jouer" et accepter l'invitation pour rejoindre la partie sauvegardée. 
 
Remarque : la pause et reprise d'une partie sauvegardée n'est possible que lors d'une partie "normale" (c'est à dire sans le mode de jeu Item ou Tournoi). Dans le cas contraire, celle-ci n'est pas possible. 
 

 
## La gestion des amis 
L'ajout d'un amis se fait par l'envoi d'une demande d'ami. Le joueur concerné doit accepter cette demande dans "accepter une demande d'ami", il doit entrer le pseudo du joueur qui l'a invité. Ainsi, les deux joueurs sont amis et il est possible de voir sa liste d'amis dans la section "Afficher ma liste d'amis". Pleins d'autres options sont également disponible comme le classement,afficher la liste des demandes d'amis, etc.. 
 
 ## Messagerie
Il est possible d'envoyer un message à un ami lorsqu'il est connecté ou non. Dans le cas ou ce dernier n'est pas connecté, il recevra quand même les messages et les verras lorsqu'il entrera dans la conversation avec le joueur concerné. Une conversation n'est pas autorisé entre 3 joueurs simultanément. Un joueur ne peut pas chatter avec un autre joueur qui chatte en même temps. Cependant, deux amis peuvent discuter entre-eux pendant que deux autres amis chattent ensemble. 

Remarque : il n'est possible à l'utilisateur de chatter avec un ami que dans cette section bien spécifique, il n'est pas autorisé à chatter lors d'une partie. 

## Partie GUI 
Toutes les fonctionnalités (les parties normales à deux joueurs ou à 4 joueurs, le mode item, le mode tournoi ainsi que la pause et reprise d'une partie sauvegardée) ont étés implémentées dans la GUI comme dans le terminal. Le classement, la gestion d'amis (Ajout et suppression d'un ami, chat, afficher liste d'amis ,etc..) fonctionnent également sans soucis. Les bugs mentionnées ci-dessus, pour la version terminal, ne s'appliquent pas à la GUI. 

En ce qui concerne l'utilisation de la GUI, celle-ci est intuitive. Pour jouer un coup lors d'une partie il suffit de cliquer sur le pion et ensuite sur la case sur laquelle le joueur veut se déplacer. Pour poser un murs, il cliquer entre les case à l'endroit souhaité.  

## Librairie graphique utilisée 
Il faut installer la libraire smfl au préalable. Il faut lancer le script configure.sh au préalable qui va lancer l'installation de la librairie sfml. Le jeu est compatible sur une machine ayant comme système d’exploitation GNU/Linux.
## Lancement de la GUI
Le lancement de la gui se fait de cette manière : 
Il faut d'abord compiler les fichiers à l'aide de la commande make -B.
Pour lancer le serveur => ./serveur \<port\>
Pour lancer le client en gui => ./client \<address ip\> \<port\> \<gui\>

En mode Item, on n'a besoin d'actionner seulement la bombe pour detruire un mur, pour l'autre item, il suffit simplement de traverser un mur.
La résolution peut être changé dans la section paramètre du menu principal.
Il n'y a aucun bug connu dans la version GUI jusqu'à aujourd'hui



## Compatibilité 
Un client en terminal peut jouer une partie, chatter, etc.. avec un client en GUI. Le serveur se comporte de la même manière avec les deux types de clients (GUI ou terminal). 

## Remarques 
Le SRD a été mis à jour en fonction des changements que nous avons effectués. 
