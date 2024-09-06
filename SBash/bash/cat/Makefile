CC = gcc -std=c11
FLAGS = -Wall -Werror -Wextra


cat : cat.c
		$(CC) $(FLAGS) cat.c -o cat

clean:
		rm -rf cat

rebuild: clean all

all : cat