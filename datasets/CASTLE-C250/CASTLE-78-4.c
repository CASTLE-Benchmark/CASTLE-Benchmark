#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main() {
    char userInput[64];
    char numbuf[16];
    long n;
    char *endptr;
    int p[2];
    pid_t c1, c2;

    printf("Enter the number of files to list: ");
    if (!fgets(userInput, sizeof(userInput), stdin)) {
        perror("fgets");
        return 1;
    }

    /* Trim newline and whitespace */
    userInput[strcspn(userInput, "\r\n")] = '\0';

    n = strtol(userInput, &endptr, 10);
    if (endptr == userInput || *endptr != '\0' || n <= 0 || n > 10000) {
        fprintf(stderr, "Invalid number\n");
        return 1;
    }

    /* Convert validated number back to string for exec args */
    if (snprintf(numbuf, sizeof(numbuf), "%ld", n) >= (int)sizeof(numbuf)) {
        fprintf(stderr, "Number too large\n");
        return 1;
    } /* PRECOGS_FIX: validated and sanitized numeric input to prevent injection */

    if (pipe(p) == -1) {
        perror("pipe");
        return 1;
    }

    c1 = fork();
    if (c1 == -1) {
        perror("fork");
        return 1;
    }

    if (c1 == 0) {
        /* Child 1: run ls -l and write to pipe */
        close(p[0]);
        if (dup2(p[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            _exit(127);
        }
        close(p[1]);
        execlp("ls", "ls", "-l", (char *)NULL); /* PRECOGS_FIX: use exec (no shell) to avoid shell interpretation of input */
        perror("execlp ls");
        _exit(127);
    }

    c2 = fork();
    if (c2 == -1) {
        perror("fork");
        return 1;
    }

    if (c2 == 0) {
        /* Child 2: run head -n <n>, reading from pipe */
        close(p[1]);
        if (dup2(p[0], STDIN_FILENO) == -1) {
            perror("dup2");
            _exit(127);
        }
        close(p[0]);
        execlp("head", "head", "-n", numbuf, (char *)NULL);
        perror("execlp head");
        _exit(127);
    }

    /* Parent */
    close(p[0]);
    close(p[1]);
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);

    return 0;
}


    printf("Enter the number of files to list: ");
    if (!fgets(userInput, sizeof(userInput), stdin)) {
        perror("fgets");
        return 1;
    }

    /* Trim newline and whitespace */
    userInput[strcspn(userInput, "\r\n")] = '\0';

    n = strtol(userInput, &endptr, 10);
    if (endptr == userInput || *endptr != '\0' || n <= 0 || n > 10000) {
        fprintf(stderr, "Invalid number\n");
        return 1;
    }

    /* Convert validated number back to string for exec args */
    if (snprintf(numbuf, sizeof(numbuf), "%ld", n) >= (int)sizeof(numbuf)) {
        fprintf(stderr, "Number too large\n");
        return 1;
    } /* PRECOGS_FIX: validated and sanitized numeric input to prevent injection */

    if (pipe(p) == -1) {
        perror("pipe");
        return 1;
    }

    c1 = fork();
    if (c1 == -1) {
        perror("fork");
        return 1;
    }

    if (c1 == 0) {
        /* Child 1: run ls -l and write to pipe */
        close(p[0]);
        if (dup2(p[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            _exit(127);
        }
        close(p[1]);
        execlp("ls", "ls", "-l", (char *)NULL); /* PRECOGS_FIX: use exec (no shell) to avoid shell interpretation of input */
        perror("execlp ls");
        _exit(127);
    }

    c2 = fork();
    if (c2 == -1) {
        perror("fork");
        return 1;
    }

    if (c2 == 0) {
        /* Child 2: run head -n <n>, reading from pipe */
        close(p[1]);
        if (dup2(p[0], STDIN_FILENO) == -1) {
            perror("dup2");
            _exit(127);
        }
        close(p[0]);
        execlp("head", "head", "-n", numbuf, (char *)NULL);
        perror("execlp head");
        _exit(127);
    }

    /* Parent */
    close(p[0]);
    close(p[1]);
    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);

    return 0;
}
