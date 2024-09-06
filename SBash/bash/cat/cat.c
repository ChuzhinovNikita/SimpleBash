#include "cat_head.h"

int main(int argc, char *argv[]) {
  int ind = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      ind = i;
      parser(argc, argv, &Options);
      output(argv, &Options, ind);
    }
  }

  return 0;
}

void parser(int argc, char *argv[], opt *Options) {
  int opt = 0, option_index = 0;

  while ((opt = getopt_long(argc, argv, "+benstvTE", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        Options->b = 1;
        break;
      case 'e':
        Options->e = 1;
        Options->v = 1;
        break;
      case 'n':
        Options->n = 1;
        break;
      case 's':
        Options->s = 1;
        break;
      case 't':
        Options->t = 1;
        Options->v = 1;
        break;
      case 'v':
        Options->v = 1;
        break;
      case 'T':
        Options->t = 1;
        break;
      case 'E':
        Options->e = 1;
        break;
      default:
        printf("Неправильный флаг\n");
        exit(1);
    }
  }
}

void output(char *argv[], opt *Options, int ind) {
  char symbol, last_symbol;
  int couter = 1, empty_count = 0;

  FILE *f = fopen(argv[ind], "r");

  if (f != NULL) {
    for (last_symbol = '\n'; (symbol = fgetc(f)) != EOF; last_symbol = symbol) {
      if (Options->b && Options->n) Options->n = 0;

      if (Options->s) {
        if (last_symbol == '\n' && symbol == '\n') {
          if (empty_count == 1) continue;
          empty_count += 1;
        } else
          empty_count = 0;
      }

      if (Options->n && Options->b == 0) {
        if (last_symbol == '\n') {
          printf("%6d\t", couter);
          couter += 1;
        }
      }

      if (Options->b) {
        if (last_symbol == '\n' && symbol != '\n') {
          printf("%6d\t", couter);
          couter += 1;
        }
      }

      if (Options->v) {
        if ((symbol >= 0 && symbol <= 31) && symbol != '\t' && symbol != '\n') {
          printf("^");
          symbol += 64;
        } else if (symbol == 127) {
          printf("^");
          symbol = '?';
        }
      }

      if (Options->e && symbol == '\n') {
        if (Options->b == 1 && last_symbol == '\n' && symbol == '\n')
          printf("      \t$");
        else
          printf("$");
      }

      if (Options->t) {
        if (symbol == 9) {
          symbol = 'I';
          printf("^");
        }
      }
      printf("%c", symbol);
    }
    fclose(f);
  } else
    fprintf(stderr, "No such file or directory: %s\n", argv[ind]);
}