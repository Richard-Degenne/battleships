game: game.o network.o overall_net.o
	gcc -o game game.o network.o overall_net.o -lpthread

game.o: game.c game.h network.o
	gcc -c game.c network.o -lpthread

network.o: network.c network.h
	gcc -c network.c

overall_net.o: overall_net.c overall_net.h
	gcc -c overall_net.c
