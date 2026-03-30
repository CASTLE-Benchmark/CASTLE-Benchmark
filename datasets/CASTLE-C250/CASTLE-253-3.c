#include <stdio.h>
#include <stdlib.h>

void echo()
{
    char dataBuffer[100] = "";
    char * data = dataBuffer;
    printf("Please enter a string: ");

    /* Correctly check fgets return value for NULL */
    if (fgets(data, 100, stdin) == NULL) // PRECOGS_FIX: check for NULL return from fgets (error/EOF)
    {
        if (feof(stdin))
        {
            fprintf(stderr, "fgets failed: EOF reached\n");
        }
        else
        {
            perror("fgets failed");
        }
        exit(1);
    }

    /* Remove trailing newline if present */
    size_t i;
    for (i = 0; i < 99 && data[i] != '\0'; ++i) { }
    if (i > 0 && data[i - 1] == '\n')
    {
        data[i - 1] = '\0';
    }

    printf("%s\n", data);
}

int main()
{
    echo();
    return 0;
}