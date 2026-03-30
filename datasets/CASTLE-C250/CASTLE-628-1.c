#include <stdio.h>
#include <stdlib.h>

void write_to_file(const char *filename, const char *text) {
    /* Validate inputs before use to avoid NULL dereference and other misuse */
    if (filename == NULL) {
        perror("Invalid filename (NULL)");
        return;
    }
    if (text == NULL) {
        fprintf(stderr, "Invalid text: NULL\n");
        return;
    }

    /* Reject overly long names and any containing path separators or ".." sequences */
    size_t len = 0;
    for (const char *p = filename; *p != '\0'; ++p) {
        if (*p == '/' || *p == '\\') { /* PRECOGS_FIX: reject directory separators to avoid path traversal */
            fprintf(stderr, "Invalid filename: contains directory separator\n");
            return;
        }
        if (*p == '.' && *(p + 1) == '.') { /* PRECOGS_FIX: reject parent-directory references */
            fprintf(stderr, "Invalid filename: contains '..'\n");
            return;
        }
        ++len;
        if (len > 255) {
            fprintf(stderr, "Invalid filename: too long\n");
            return;
        }
    }

    char safe_path[512];
    if (snprintf(safe_path, sizeof(safe_path), "./%s", filename) >= (int)sizeof(safe_path)) {
        fprintf(stderr, "Path too long\n");
        return;
    }

    FILE *file = fopen(safe_path, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    if (fprintf(file, "%s", text) < 0) {
        perror("Error writing to file");
        fclose(file);
        return;
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

#include <stdio.h>
#include <string.h>

int main() {
    const char *filename = "output.txt";
    const char *text = "Hello, this is a sample text written to the file.";

    // PRECOGS_FIX: Ensure filename is not externally controlled
    if (strstr(filename, "..") || strstr(filename, "/") || strstr(filename, "\\")) {
        printf("Invalid filename\n");
        return 1;
    }

    // PRECOGS_FIX: Correct argument order for write_to_file
    write_to_file(filename, text); // Corrected order: filename first, then text

    printf("Text written to %s\n", filename);

    return 0;
}