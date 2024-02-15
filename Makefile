CC = g++ -std=c++2a -g
CFLAGS =-Wall #-O3 -pthread -masm=intel  -mlong-double-128 -ggdb3 -Wall -lquadmath # -Wpedantic -Wextra -Weffc++ #-Wsign-conversion -Wconversion  -Wold-style-cast -Wzero-as-null-pointer-constant -Wsuggest-final-types -Woverloaded-virtual -Wsuggest-final-methods-Wstrict-null-sentinel -Wnoexcept -Wctor-dtor-privacy -Wsign-promo -Wsuggest-override -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable
SRCSJEU := $(wildcard src/serveur/modedejeu/Jeu/src/*.cpp)
SRCSJEU2 := $(wildcard src/serveur/modedejeu/JeuItem/src/*.cpp)
SRCSERVEUR = src/serveur/main.o src/serveur/Serveur.o src/common/common.o src/serveur/basededonnee/BaseDeDonnee.o src/serveur/Partie.o src/serveur/Tournoi.o src/serveur/User.o 
LDFLAGS_CLIENT = -lncursesw -pthread -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
LDFLAGS_SERVEUR =
SRCSCLIENT := $(shell find ./src/clientSrc/ -type f -iname '*.cpp')
OBJSCLIENT=$(foreach x, $(basename $(SRCSCLIENT)), $(x).o)

all: serveur client

%/main.o: %/main.cpp
	$(CC) -o $@ -c $<


%.o: %.cpp %.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

client: $(OBJSCLIENT) ./src/common/common.o
	$(CC) -o $@ $^ $(LDFLAGS_CLIENT)

serveur: $(SRCSERVEUR) $(SRCSJEU) $(SRCSJEU2)
	make -C src/serveur/modedejeu/Jeu/
	make -C src/serveur/modedejeu/JeuItem/
	$(CC) -o $@ $(SRCSERVEUR) src/serveur/modedejeu/Jeu/Case.o src/serveur/modedejeu/Jeu/Jeu.o src/serveur/modedejeu/Jeu/Mur.o src/serveur/modedejeu/Jeu/Pion.o src/serveur/modedejeu/Jeu/Plateau.o src/serveur/modedejeu/JeuItem/caseItem.o src/serveur/modedejeu/JeuItem/JeuItem.o src/serveur/modedejeu/JeuItem/PionItem.o src/serveur/modedejeu/JeuItem/PlateauItem.o src/serveur/modedejeu/JeuItem/Item.o $(LDFLAGS_SERVEUR)


cleanClient:
	rm -f client $(OBJSCLIENT)

cleanServeur:
	rm -f serveur $(SRCSERVEUR)
	make -C src/serveur/modedejeu/Jeu/ clean
	make -C src/serveur/modedejeu/JeuItem/ clean

clean: cleanClient cleanServeur
	rm -f rm -f $(shell find . -type f -name '*.o')

re: clean all
# fclean: clean
# 	rm serveur

