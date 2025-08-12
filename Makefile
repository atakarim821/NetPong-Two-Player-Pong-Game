all: pingpong

pingpong: src/pingpong.c src/game.c src/net.c
	gcc -Iinclude -pthread -o pingpong src/pingpong.c src/game.c src/net.c -lncurses

clean:
	rm -f pingpong

