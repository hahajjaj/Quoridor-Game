# Quoridor Game

## Launching Executables
To compile all files, the make command must be used in the terminal.
Server:

./serveur \<port\>
 
Client : 

./client \<address ip\> \<port\> \<terminal/gui\>
 
terminal = main functional interface (default).
terminal2 = old interface with some display bugs related to ncurses library.

 
## Terminal Size

It must be large enough to properly display the menu and the game board. Changing the terminal size while the application is running can cause display issues. Therefore, make sure to open a sufficiently large terminal before launching the application.

## Visual

<p align="center">
  <img src="https://github.com/hahajjaj/Quoridor-Game/blob/main/imgs/login_screen.png" alt="logscreen" width="280" height="auto"/>
  <img src="https://github.com/hahajjaj/Quoridor-Game/blob/main/imgs/login_info_screen.png" alt="logscreen2" width="280" height="auto"/>
  <img src="https://github.com/hahajjaj/Quoridor-Game/blob/main/imgs/main_menu.png" alt="mainMenu" width="280" height="auto"/>
</p>
<p align="center">
  <img src="https://github.com/hahajjaj/Quoridor-Game/blob/main/imgs/lobby.png" alt="lobby" width="280" height="auto"/>
  <img src="https://github.com/hahajjaj/Quoridor-Game/blob/main/imgs/friend_list.png" alt="friendlist" width="280" height="auto"/>
  <img src="https://github.com/hahajjaj/Quoridor-Game/blob/main/imgs/chat_window.png" alt="mainMenu" width="280" height="auto"/>
</p>
<p align="center">
  <img src="https://github.com/hahajjaj/Quoridor-Game/blob/main/imgs/game.png" alt="lobby" width="300" height="auto"/>
</p>

## Game Instructions

To play, the opposing player must be in the friend list. The user must go to "Play", "Create a game", and configure the game options (i.e., game mode, number of players). They must then invite a player (if it is a 2-player game) or 3 players (in the case of a 4-player game). Finally, they must press "Start game". They will be put on hold until the opposing player joins. 
 
Once the invitation is sent, the opponent can find it in the "View game invitations" section.
To join the game, the opponent's username must be entered. And finally, the game can begin.

There are 3 possible game modes:

* Play a normal game in 1 vs 1 (two-player game) or 1 vs 1 vs 1 vs 1 (four-player game).
* Play a game in Item mode (two items available, wall removal with the s key, and wall jumping)
* Play in tournament mode

Use the cursor to move a pawn

* Use the arrows to move the cursor
* Use the Enter key to confirm a choice


To switch the cursor to wall mode, use the m key. Once done, you can rotate the wall using the r key.

A game can be paused using the x key. It will be saved and can be resumed later via the "resume a saved game" option. These players will then be invited to rejoin the saved game and must do so via the "View my game invitations" option. The game will start once all players have accepted the invitation.
 
 ## Tournaments
 
 To launch tournament mode, it must be selected when creating a game in the "game mode" section. Then the organizer must invite all the tournament players and finally start their game.

In tournament mode, players invited by the organizer are placed in 1 vs 1 games.
If the number of players is odd, the extra player is considered the default winner.
When all the games are finished, a new round starts with the winners of the last round.

 ## Item
In Item game mode, two items are available. One offers the possibility to delete a wall by pressing the "s" key on the keyboard. And the other allows crossing a single wall. Items are usable only once; once consumed, the power provided by the item is no longer active.
 
 
 ## Pause and Resume a Saved Game
 In-game, to pause, the player must press the "x" key. The game is then ended and saved. Both players can resume it; for this, a player must go to the "resume a saved game" option and must choose the saved game to resume. They select the game they want to resume using the arrows. The concerned players will receive an invitation request and must go to the "View my game invitations" section and accept the invitation to join the saved game.

Note: pausing and resuming a saved game is only possible during a "normal" game (i.e., without the Item or Tournament game mode). Otherwise, it is not possible.
 

 
## Friend Management
Adding a friend is done by sending a friend request. The concerned player must accept this request in "accept a friend request," they must enter the username of the player who invited them. Thus, the two players are friends, and it is possible to see their friend list in the "Display my friend list" section. Many other options are also available, such as ranking, displaying the list of friend requests, etc..
 
 ## Messaging
It is possible to send a message to a friend whether they are connected or not. In case the latter is not connected, they will still receive the messages and see them when they enter the conversation with the concerned player. A conversation is not allowed between 3 players simultaneously. A player cannot chat with another player who is chatting at the same time. However, two friends can talk to each other while two other friends chat together.

Note: it is only possible for the user to chat with a friend in this specific section; it is not authorized to chat during a game. 

## GUI Part
All functionalities (normal games for two or four players, item mode, tournament mode, as well as pause and resume a saved game) have been implemented in the GUI as in the terminal. Ranking, friend management (adding and removing a friend, chat, display friend list, etc.) also work without any issues. The bugs mentioned above for the terminal version do not apply to the GUI.

Regarding the use of the GUI, it is intuitive. To play a move during a game, just click on the pawn and then on the square to which the player wants to move. To place walls, click between the squares at the desired location.  

## Graphical Library Used
The smfl library must be installed beforehand. The configure.sh script must be run beforehand, which will launch the installation of the sfml library. The game is compatible on a machine with GNU/Linux as the operating system.
## Launching the GUI
The GUI is launched in this way:
First, compile the files using the make -B command.
To launch the server => ./server <port>
To launch the client in gui => ./client <ip address> <port> <gui>

In Item mode, only the bomb needs to be activated to destroy a wall; for the other item, simply cross a wall.
The resolution can be changed in the settings section of the main menu.
There are no known bugs in the GUI version to date



## Compatibility
A terminal client can play a game, chat, etc., with a GUI client. The server behaves the same way with both types of clients (GUI or terminal).
