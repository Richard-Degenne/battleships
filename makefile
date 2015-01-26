all: client/main serv/serv

client/main: client/main.c client/game.o client/network.o client/overall_net.o
	gcc -o client/main client/main.c client/game.o client/network.o client/overall_net.o -lpthread

client/game.o: client/game.c client/game.h client/network.o
	gcc -c -o client/game.o client/game.c client/network.o -lpthread

client/network.o: client/network.c client/network.h
	gcc -c -o client/network.o client/network.c

client/overall_net.o: client/overall_net.c client/overall_net.h
	gcc -c -o client/overall_net.o client/overall_net.c

serv/serv: server/serv.c server/serv.h
	gcc -o server/serv server/serv.c -lpthread

.PHONY: clean

clean:
	rm client/*.o client/main server/serv
