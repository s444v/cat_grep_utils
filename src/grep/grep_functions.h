#ifndef GREP_FUNCTIONS_H
#include <regex.h>
typedef struct struct_options {
  int e, i, v, c, l, n, h, s, o, f;
} all_options;
void output(char *filename, all_options *options, char *pattern,
            int count_files);
void flag_checker(int rez, all_options *options, int *flag);
int index_checker(int argc, char *argv[]);
int find_pattern(regex_t re, char *buffer);
void c_option(int *count_lines);
void n_option(int line_number, char *pattern, int compare, regex_t re,
              char *buffer, int count_files, char *filename);
void o_option(regex_t re, char *buffer, char *pattern, int compare,
              char *filename, int count_files);
void regexs(regex_t *re, all_options *options, char *pattern);
void output_pattern(int len, char *buffer, regmatch_t *pmatch);
void last_output(all_options *options, char *filename, int count_files,
                 char *buffer, int count_lines);
void output_buffer(int count_files, all_options *options, char *filename,
                   char *buffer);
void n_output(all_options *options, int line_number, char *filename,
              char *buffer, int count_files, regex_t re, char *pattern);
void flags_variables(all_options *options, char *pattern, int *compare,
                     regex_t *re, int *count_files);
#endif