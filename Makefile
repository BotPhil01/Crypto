CC=gcc

prod: 
	$(CC) -o prod main.c
debug:
	$(CC) -g -o debug main.c

clean:
	rm -f prod debug
