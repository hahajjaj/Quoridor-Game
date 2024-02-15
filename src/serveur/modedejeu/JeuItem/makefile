FLAGS=-Wall #-pedantic
SRCS := $(wildcard src/*.cpp)
SRCS2 := $(wildcard ../Jeu/src/*.cpp)
OBJS=$(foreach x, $(basename $(SRCS)), $(notdir $(x)).o)
GXX = g++ -std=c++2a -g

all: $(OBJS) #quoridor
# all: quoridor


quoridor: $(SRCS) $(SRCS2)
	$(GXX) $(SRCS) $(SRCS2) -o quoridor $(FLAGS)

clean:
	rm -f Quoridor

%.o: ./src/%.cpp ./header/%.hpp
	$(GXX) -c -o $@ $< $(FLAGS)