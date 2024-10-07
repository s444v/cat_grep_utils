#include <getopt.h>
#include <stdio.h>

#include "cat_functions.h"

int main(int argc, char *argv[]) {
  const struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                        {"number", 0, NULL, 'n'},
                                        {"squeeze-blank", 0, NULL, 's'},
                                        {NULL, 0, NULL, 0}};
  int rez;
  all_options options = {0, 0, 0, 0, 0, 0, 0, 0};
  int flag_invalid = 1;

  if (index_checker(argc, argv) != 0) {
    while ((rez = getopt_long(index_checker(argc, argv), argv, "beEnstTv",
                              long_options, NULL)) != -1) {
      flag_checker(rez, &options, &flag_invalid);
    }
    if (flag_invalid) {
      int count_lines = 1;
      int flag_end_line = 1;
      int flag_blanks = 1;
      for (int i = index_checker(argc, argv); i < argc; i++) {
        output(argv[i], &options, &count_lines, &flag_end_line, &flag_blanks);
      }
    }
  } else {
    while ((rez = getopt_long(argc, argv, "beEnstTv", long_options, NULL)) !=
           -1) {
      flag_checker(rez, &options, &flag_invalid);
    }
    if (flag_invalid) {
      output_stdin(&options);
    }
  }
  return 0;
}
