FLAGS=-Wall #-pedantic
SRCS := $(wildcard src/*.cpp)
OBJS=$(foreach x, $(basename $(SRCS)), $(notdir $(x)).o)
GXX = g++ -std=c++2a -g

all: $(OBJS) #quoridor
# all: quoridor


quoridor: $(SRCS)
	$(GXX) $(SRCS) ../../../common/common.cpp -o quoridor $(FLAGS) 

clean:
	rm -f quoridor

%.o: ./**/%.cpp
	$(GXX) -c -o $@ $< $(FLAGS)
