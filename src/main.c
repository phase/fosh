#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "builtins.h"

int launch(char** args) {
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("fosh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fosh");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int execute(char** args) {
    if (args[0] == NULL) {
        return 1;
    }

    // search through builtins
    for (int i = 0; i < builtin_number(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_fn[i])(args);
        }
    }

    return launch(args); 
}

#define RL_BUFSIZE 1024

char* read_line() {
    int bufsize = RL_BUFSIZE;
    int pos = 0;
    char* buffer = malloc(sizeof(char) * bufsize);

    if (!buffer) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int c = getchar();

        if (c == EOF || c == '\n') {
            // replace eof/newline with null character
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }
        pos++;

        if (pos >= bufsize) {
            bufsize += RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

char** split_line(char* line) {
    int bufsize = TOK_BUFSIZE;
    int pos = 0;
    char** tokens = malloc(sizeof(char) * bufsize);
    char* token;
    char** tokens_backup;

    if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;

        if (pos >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, sizeof(char*) * bufsize);
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }

    tokens[pos] = NULL;
    return tokens;
}

void loop() {
    int status;
    do {
        printf("$ ");
        char* line = read_line();
        char** tokens = split_line(line);

        status = execute(tokens);

        free(line);
        free(tokens);
    } while(status);
}

int main(int argc, char** argv) {
    loop();
    return EXIT_SUCCESS;
}
