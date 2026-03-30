#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contains_char(char c, const char *target){
    char *str;
    str = (char*)malloc((strlen(target) + 1) * sizeof(char));
    if (str == NULL) {
        printf("Failed to allocate memory.\n");
        exit(1); // Indicate error and exit
    }
    strcpy(str, target);
    char *original_str = str; // PRECOGS_FIX: keep a reference to the start of the buffer
    while (*str != 0) {
        if( *str == c ){
            free(original_str); // PRECOGS_FIX: free the original pointer
            return 1;
        }

        str = str + 1;
    }

    free(original_str); // PRECOGS_FIX: free the original pointer
    return 0;
}
}

int main() {
    char search = 'a';
    char *text = "banana";

    if (contains_char(search, text)) {
        printf("'%s' contains '%c' character.\n", text, search);
    }
    else {
        printf("'%s' does not contain '%c' character.\n", text, search);
    }
    return 0;
}