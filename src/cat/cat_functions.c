#include "cat_functions.h"

#include <stdio.h>

int index_checker(int argc, char *argv[]) {
  int index = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      index = i;
      i += argc;
    }
  }
  return index;
}

void flag_checker(int rez, all_options *options, int *flag_invalid) {
  if (rez == 'b')
    options->b = 1;
  else if (rez == 'e')
    options->e = 1;
  else if (rez == 'E')
    options->E = 1;
  else if (rez == 'n')
    options->n = 1;
  else if (rez == 's')
    options->s = 1;
  else if (rez == 't')
    options->t = 1;
  else if (rez == 'T')
    options->T = 1;
  else if (rez == 'v')
    options->v = 1;
  else
    *flag_invalid = 0;
}

void output_stdin(all_options *options) {
  int flag_count = 1;
  int count_lines = 1;
  int flag_blanks = 1;
  while (1) {
    char s = getchar();
    if (options->s == 1 && s_output(s, &flag_blanks)) {
      continue;
    }
    if (options->b == 1) {
      b_output(s, &flag_count, &count_lines);
    }
    if (options->n == 1 && options->b == 0) {
      n_output(s, &flag_count, &count_lines);
    }
    if (options->E == 1 || options->e == 1) {
      e_output(s);
    }
    if ((options->T == 1 || options->t == 1) && t_output(s)) {
      continue;
    }
    if ((options->v == 1 || options->t == 1 || options->e == 1) &&
        v_output(s)) {
      continue;
    }
    printf("%c", s);
  }
}

void output(char *filename, all_options *options, int *count_lines,
            int *flag_end_line, int *flag_blanks) {
  char buffer;
  FILE *fp = fopen(filename, "r");
  if (fp) {
    while ((buffer = fgetc(fp)) != EOF) {
      if (options->s == 1 && s_output(buffer, flag_blanks)) {
        continue;
      }
      if (options->b == 1) {
        b_output(buffer, flag_end_line, count_lines);
      }
      if (options->n == 1 && options->b == 0) {
        n_output(buffer, flag_end_line, count_lines);
      }
      if (options->E == 1 || options->e == 1) {
        e_output(buffer);
      }
      if ((options->T == 1 || options->t == 1) && t_output(buffer)) {
        continue;
      }
      if ((options->v == 1 || options->t == 1 || options->e == 1) &&
          v_output(buffer)) {
        continue;
      }
      printf("%c", buffer);
    }
    fclose(fp);
  } else
    printf("\nCannot open this file \"%s\"\n", filename);
}

int s_output(char buffer, int *flag_blanks) {
  int next = 0;
  if (buffer != '\n') {
    *flag_blanks = 0;
  } else if (buffer == '\n' && *flag_blanks == 0) {
    *flag_blanks = 1;
  } else if (buffer == '\n' && *flag_blanks == 1) {
    *flag_blanks = 2;
  } else {
    next = 1;
  }
  return next;
}

void b_output(char buffer, int *flag_end_line, int *count_lines) {
  if (*flag_end_line == 1 && buffer != '\n') {
    printf("%6d\t", *count_lines);
    *flag_end_line = 0;
    *count_lines += 1;
  }
  if (buffer == '\n' && *flag_end_line == 0) {
    *flag_end_line = 1;
  }
}

void n_output(char buffer, int *flag_end_line, int *count_lines) {
  if (*flag_end_line == 1) {
    printf("%6d\t", *count_lines);
    *flag_end_line = 0;
    *count_lines += 1;
  }
  if (buffer == '\n' && *flag_end_line == 0) {
    *flag_end_line = 1;
  }
}

void e_output(char buffer) {
  if (buffer == '\n') printf("$");
}

int t_output(char buffer) {
  int next = 0;
  if (buffer == '\t') {
    printf("^I");
    next = 1;
  }
  return next;
}

int v_output(char buffer) {
  int next = 0;
  if (buffer < -96) {
    printf("M-^%c", buffer + 192);
    next = 1;
  } else if (buffer < 0) {
    printf("M-%c", buffer + 128);
    next = 1;
  } else if (buffer == 10 || buffer == 9)
    buffer = buffer;
  else if (buffer < 32) {
    printf("^%c", buffer + 64);
    next = 1;
  } else if (buffer < 127)
    buffer = buffer;
  else {
    printf("^?");
    next = 1;
  }
  return next;
}
