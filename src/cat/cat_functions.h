#ifndef CAT_FUNCTIONS_H
typedef struct struct_options {
  int b, e, E, n, s, t, T, v;
} all_options;
int index_checker(int argc, char *argv[]);
void flag_checker(int rez, all_options *flags, int *flag);
void output(char *filename, all_options *flags, int *count, int *flag_end_line,
            int *flag_blanks);
void output_stdin(all_options *flags);
int s_output(char buffer, int *flag);
void b_output(char buffer, int *flag_count, int *count);
void n_output(char buffer, int *flag_count, int *count);
void e_output(char buffer);
int t_output(char buffer);
int v_output(char buffer);
#endif