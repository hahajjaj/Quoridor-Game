FLAGS=-Wall #-pedantic
SRCS2 := $(wildcard ./*.cpp)
OBJS=$(foreach x, $(basename $(SRCS)), $(notdir $(x)).o)
GXX = g++ -std=c++2a -g

all: main

main: main.cpp Board.cpp Board.hpp Case.hpp Case.cpp Wall.hpp Wall.cpp Pion.hpp Pion.cpp Animation/Animation.cpp Animation/AnimatedSprite.cpp GestionDesMenus/Menu.hpp GestionDesMenus/Menu.cpp GestionDesMenus/Button.hpp GestionDesMenus/MenuAmi/menu_ami.hpp GestionDesMenus/MenuAmi/menu_ami.cpp GestionDesMenus/MenuAmi/rowFriend.cpp GestionDesMenus/MenuAmi/rowRequest.cpp
	g++ main.cpp Board.cpp Case.cpp Wall.cpp Pion.cpp Animation/Animation.cpp Animation/AnimatedSprite.cpp GestionDesMenus/Menu.cpp GestionDesMenus/MenuAmi/menu_ami.cpp GestionDesMenus/MenuAmi/rowFriend.cpp GestionDesMenus/MenuAmi/rowRequest.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
clean:
	rm -f main