#include "grep_functions.h"

#include <regex.h>
#include <stdio.h>
#include <string.h>

void flag_checker(int rez, all_options *options, int *flag) {
  if (rez == 'e')
    options->e = 1;
  else if (rez == 'i')
    options->i = 1;
  else if (rez == 'v')
    options->v = 1;
  else if (rez == 'c')
    options->c = 1;
  else if (rez == 'l')
    options->l = 1;
  else if (rez == 'n')
    options->n = 1;
  else if (rez == 'h')
    options->h = 1;
  else if (rez == 's')
    options->s = 1;
  else if (rez == 'o')
    options->o = 1;
  else if (rez == 'f')
    options->f = 1;
  *flag = 1;
}

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

void output(char *filename, all_options *options, char *pattern,
            int count_files) {
  char buffer[1024];
  int count_lines = 0;
  int line_number = 0;
  regex_t re;
  int compare = 1;
  flags_variables(options, pattern, &compare, &re, &count_files);
  FILE *fp = fopen(filename, "r");
  if (fp) {
    while (fgets(buffer, 1024, fp) != NULL) {
      line_number++;
      if (options->l == 1 && find_pattern(re, buffer) != options->v) {
        printf("%s\n", filename);
        break;
      } else if (options->l == 1 && find_pattern(re, buffer) == options->v) {
        continue;
      }
      if (options->c == 1 && find_pattern(re, buffer) != options->v) {
        c_option(&count_lines);
        continue;
      }
      if (options->n == 1 && find_pattern(re, buffer) != options->v) {
        n_output(options, line_number, filename, buffer, count_files, re,
                 pattern);
        continue;
      }
      if (options->o == 1) {
        o_option(re, buffer, pattern, compare, filename, count_files);
        continue;
      }
      if (options->f == 1) {
        continue;  // не сделан флаг -f
      }
      if (find_pattern(re, buffer) == options->v) {
        continue;
      }
      output_buffer(count_files, options, filename, buffer);
    }
    fclose(fp);
    last_output(options, filename, count_files, buffer, count_lines);
  } else if (options->s == 0) {
    printf("file not found");
  }
  regfree(&re);
}

void flags_variables(all_options *options, char *pattern, int *compare,
                     regex_t *re, int *count_files) {
  if (options->i) {
    regcomp(re, pattern, REG_ICASE);
  } else {
    regcomp(re, pattern, REG_EXTENDED);
  }
  if (options->v == 1) {
    *compare = 0;
  }
  if (options->h == 1) {
    *count_files = 1;
  }
}

void n_output(all_options *options, int line_number, char *filename,
              char *buffer, int count_files, regex_t re, char *pattern) {
  if (options->o == 1) {
    n_option(line_number, pattern, options->v, re, buffer, count_files,
             filename);
  } else {
    if (count_files > 2)
      printf("%s:%d:%s", filename, line_number, buffer);
    else
      printf("%d:%s", line_number, buffer);
  }
}

void output_buffer(int count_files, all_options *options, char *filename,
                   char *buffer) {
  if (count_files > 2 && options->h != 1) {
    printf("%s:%s", filename, buffer);
  } else {
    printf("%s", buffer);
  }
}

void last_output(all_options *options, char *filename, int count_files,
                 char *buffer, int count_lines) {
  if (options->i == 1 && (strchr(buffer, '\n') == NULL)) {
    printf("\n");
  }
  if (options->n == 1 && (strchr(buffer, '\n') == NULL)) {
    printf("\n");
  }
  if (options->c == 1 && options->l != 1) {
    if (count_files > 2) {
      printf("%s:%d\n", filename, count_lines);
    } else {
      printf("%d\n", count_lines);
    }
  }
}

int find_pattern(regex_t re, char *buffer) {
  int next = 1;
  if (regexec(&re, buffer, 0, NULL, 0)) {
    next = 0;
  }
  return next;
}

void n_option(int line_number, char *pattern, int compare, regex_t re,
              char *buffer, int count_files, char *filename) {
  regmatch_t pmatch[1];
  int len = strlen(pattern);
  if (!compare) {
    while (regexec(&re, buffer, 1, pmatch, 0) == 0) {
      if (count_files > 2) {
        printf("%s:%d:", filename, line_number);
        output_pattern(len, buffer, pmatch);
      } else {
        printf("%d:", line_number);
        output_pattern(len, buffer, pmatch);
      }
      buffer += pmatch[0].rm_eo;
    }
  }
}

void c_option(int *count_lines) { *count_lines += 1; }

void o_option(regex_t re, char *buffer, char *pattern, int compare,
              char *filename, int count_files) {
  regmatch_t pmatch[1];
  int len = strlen(pattern);
  if (compare) {
    while (regexec(&re, buffer, 1, pmatch, 0) == 0) {
      if (count_files > 2) {
        printf("%s:", filename);
        output_pattern(len, buffer, pmatch);
      } else
        output_pattern(len, buffer, pmatch);
      buffer += pmatch[0].rm_eo;
    }
  }
}

void output_pattern(int len, char *buffer, regmatch_t *pmatch) {
  for (int i = 0; i < len; i++) {
    printf("%c", buffer[pmatch[0].rm_eo - len + i]);
  }
  printf("\n");
}