#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

struct Options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
};

typedef struct Options opt;

opt Options = {0};

struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                {"number", no_argument, NULL, 'n'},
                                {"squeeze-blank", no_argument, NULL, 's'},
                                {"show-nonprinting", no_argument, NULL, 'v'},
                                {"show-tabs", no_argument, NULL, 'T'},
                                {"show-ends", no_argument, NULL, 'E'},
                                {"help", no_argument, NULL, 0},
                                {NULL, 0, NULL, 0}};

void parser(int argc, char *argv[], opt *Options);
void output(char *argv[], opt *Options, int ind);

#endif