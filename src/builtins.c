#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtins.h"

char* builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_fn[]) (char**) = {
    &sh_cd,
    &sh_help,
    &sh_exit,
};

int sh_cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("fosh");
        }
    }
    return 1;
}

int sh_help(char** args) {
    printf("fosh - Fowler Open Shell\n\n Builtins:\n");

    for (int i = 0; i < builtin_number(); i++) {
        printf("  %s\n", builtin_str[i]);
    }
    return 1;
}

int sh_exit(char** args) {
    return 0;
}

int builtin_number() {
    return sizeof(builtin_str) / sizeof(char*);
}
