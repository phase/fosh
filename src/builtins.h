#ifndef BUILTINS_H
#define BUILTINS_H

int sh_cd(char** args);
int sh_help(char** args);
int sh_exit(char** args);
int builtin_number();

extern char* builtin_str[];

extern int (*builtin_fn[]) (char**);
#endif
