#define _GNU_SOURCE

#include "grep_head.h"

int main(int argc, char *argv[]) {
  parsser(argc, argv, &Arguments);

  for (int i = optind; i < argc; i++) {
    FILE *f = fopen(argv[i], "r");

    if (f == NULL) {
      if (!Arguments.s) {  // ФЛАГ S
        perror(argv[i]);
      }
      continue;
    }
    get_line(argc, argv[i], &Arguments, f);
  }

  regfree(&re);
  return 0;
}

void parsser(int argc, char *argv[], arg *Arguments) {
  int opt = 0;

  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        Arguments->e = 1;
        pattern_add(Arguments, optarg);
        break;
      case 'i':
        Arguments->i = 1;
        break;
      case 'v':
        Arguments->v = 1;
        break;
      case 'c':
        Arguments->c = 1;
        break;
      case 'l':
        Arguments->l = 1;
        break;
      case 'n':
        Arguments->n = 1;
        break;
      case 'h':
        Arguments->h = 1;
        break;
      case 's':
        Arguments->s = 1;
        break;
      case 'f':
        Arguments->f = 1;
        pattern_from_file(Arguments, optarg);
        break;
      case 'o':
        Arguments->o = 1;
        break;
      default:
        printf("error\n");
        break;
    }
  }

  if (Arguments->len_pattern == 0) {
    pattern_add(Arguments, argv[optind]);
    optind++;
  }
}

void priority_flags(arg *Arguments) {
  if ((Arguments->e && Arguments->l && Arguments->v) ||
      (Arguments->e && (Arguments->l || Arguments->v))) {
    Arguments->e = 0;  // приоритетность l и v перед e
  }

  if ((Arguments->l && Arguments->v) || (Arguments->l || Arguments->v)) {
    Arguments->o = 0;  // приоритетность l и v перед o
  }
}

void pattern_from_file(arg *Arguments, char *filepatch) {
  FILE *file = fopen(filepatch, "r");
  if (file == NULL) {
    if (!Arguments->s) {
      perror(filepatch);
    }
    exit(1);
  }

  char *line = NULL;
  size_t mem_line = 0;

  int read = getline(&line, &mem_line, file);

  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    pattern_add(Arguments, line);
    read = getline(&line, &mem_line, file);
  }

  free(line);
  fclose(file);
}

void pattern_add(arg *Arguments, char *pattern) {
  if (Arguments->len_pattern != 0) {
    strcat(Arguments->pattern + Arguments->len_pattern, "|");
    Arguments->len_pattern++;
  }

  Arguments->len_pattern +=
      sprintf(Arguments->pattern + Arguments->len_pattern, "(%s)", pattern);
}

void get_line(int argc, char *patch, arg *Arguments, FILE *f) {
  int pathes = argc - optind, read = 0, error = 0, line_count = 1,
      count_matches = 0;
  char *line = NULL;
  size_t mem_line = 0;

  if (Arguments->i) {  // ФЛАГ I
    error = regcomp(&re, Arguments->pattern, REG_EXTENDED | REG_ICASE);
  } else {
    error = regcomp(&re, Arguments->pattern, REG_EXTENDED | 0);
  }

  if (error) perror("Error");

  read = getline(&line, &mem_line, f);

  while (read != -1) {
    int result = regexec(&re, line, 0, NULL, 0);

    priority_flags(Arguments);

    if ((result == 0 && !Arguments->v) ||
        (Arguments->v && result != 0)) {  // ФЛАГ V

      if (!Arguments->c && !Arguments->l && !Arguments->o) {
        if (pathes > 1 && !Arguments->h) printf("%s:", patch);

        if (Arguments->n) printf("%d:", line_count);  // ФЛАГ N  и H

        print_line(line, read);
      }
      count_matches += 1;
    }

    if (result == 0 && Arguments->o && !Arguments->c) {  // ФЛАГ O  и H
      print_flag_o(Arguments, &re, line, patch, pathes, line_count);
    }

    read = getline(&line, &mem_line, f);
    line_count += 1;
  }

  if (Arguments->c) {
    print_flag_c(Arguments, count_matches, pathes, patch);
  }

  if (Arguments->l && count_matches > 0) printf("%s\n", patch);  // ФЛАГ L

  free(line);
  fclose(f);
}

void print_flag_c(arg *Arguments, int count_matches, int pathes, char *patch) {
  if (Arguments->l && count_matches > 0) count_matches = 1;

  if (pathes > 1 && !Arguments->h) {
    printf("%s:%d\n", patch, count_matches);
  } else {
    printf("%d\n", count_matches);
  }  // ФЛАГ C и H
}

void print_line(char *line, int read) {
  for (int i = 0; i < read; i++) {
    printf("%c", line[i]);
  }
  if (line[read - 1] != '\n') {
    printf("\n");
  }
}

void print_flag_o(arg *Arguments, regex_t *re, char *line, char *path,
                  int pathes, int line_count) {
  regmatch_t match;
  int offset = 0;

  while (1) {
    int result = regexec(re, line + offset, 1, &match, 0);

    if (result != 0) {
      break;
    }

    if (pathes > 1 && !Arguments->h) {
      printf("%s:", path);
    }

    if (Arguments->n) {
      printf("%d:", line_count);
    }

    for (int i = match.rm_so; i < match.rm_eo; i++) {
      printf("%c", line[offset + i]);
    }
    printf("\n");

    offset += match.rm_eo;
  }
}