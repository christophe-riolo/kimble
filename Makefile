CC = g++
FLAGS = -g -Wall --pedantic
HDRS = kimble.hpp
SRCS = kimble.cpp game.cpp peg.cpp player.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = kimble

$(MAIN): $(OBJS)
	$(CC) $(FLAGS) -o $(MAIN) $(OBJS)

.cpp.o: 
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f *.o *~

mrproper: clean
	rm $(MAIN)
