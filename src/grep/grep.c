#include <getopt.h>
#include <stdio.h>

#include "grep_functions.h"

int main(int argc, char *argv[]) {
  int rez;
  int flag_invalid = 0;
  all_options options = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  if ((argc - index_checker(argc, argv)) >= 2) {
    while ((rez = getopt(index_checker(argc, argv), argv, "eivclnhsof")) !=
           -1) {
      flag_checker(rez, &options, &flag_invalid);
    }
    if (flag_invalid) {
      for (int i = index_checker(argc, argv) + 1; i < argc; i++) {
        output(argv[i], &options, argv[index_checker(argc, argv)],
               argc - index_checker(argc, argv));
      }
    } else {
      options.e = 1;
      for (int i = index_checker(argc, argv) + 1; i < argc; i++) {
        output(argv[i], &options, argv[index_checker(argc, argv)],
               argc - index_checker(argc, argv));
      }
    }
  } else
    printf("Usage: grep [options] template [file_name]\n");
  return 0;
}