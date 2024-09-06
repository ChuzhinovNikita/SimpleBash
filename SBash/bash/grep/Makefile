CC = gcc -std=c11
FLAGS = -Wall -Werror -Wextra


s21_grep : grep.c
		$(CC) $(FLAGS) grep.c -o grep

clean:
		rm -rf grep

rebuild: clean all

all : grep