#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Arguments {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;

  char pattern[11000];
  int len_pattern;
};

typedef struct Arguments arg;

arg Arguments = {0};

regex_t re;

void parsser(int argc, char *argv[], arg *Arguments);
void priority_flags(arg *Arguments);
void pattern_from_file(arg *Arguments, char *filepatch);
void pattern_add(arg *Arguments, char *pattern);
void get_line(int argc, char *patch, arg *Arguments, FILE *f);
void print_flag_c(arg *Arguments, int count_matches, int pathes, char *patch);
void print_flag_o(arg *Arguments, regex_t *re, char *line, char *path,
                  int pathes, int line_count);
void print_line(char *line, int read);

#endif