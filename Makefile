CC=gcc
FILES = main.c helpers/blocks.c helpers/padding.c helpers/types.c helpers/printer.c

prod:
	$(CC) -o prod $(FILES)
debug:
	$(CC) -g -Wno-discarded-qualifiers -o debug $(FILES)
clean:
	rm -f prod debug
