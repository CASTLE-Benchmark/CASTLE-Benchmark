// echo -e "USERNAME=apple\nPASSWORD=secret123" > config.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConfigEntry {
    char* name;
    char* value;
    struct ConfigEntry* next;
} ConfigEntry;

ConfigEntry* createConfigEntry(char* name, unsigned int nameSize, char* value, unsigned int valueSize) {
    ConfigEntry* entry = (ConfigEntry*)malloc(sizeof(ConfigEntry));
    if (entry == NULL) {
        return NULL;
    }

    entry->name = (char*)malloc(nameSize + 1);
    entry->value = (char*)malloc(valueSize + 1);
    entry->next = NULL;
    if (entry->name == NULL || entry->value == NULL) {
        free(entry->name);
        free(entry->value);
        free(entry);
        return NULL;
    }

    strncpy(entry->name, name, nameSize);
    strncpy(entry->value, value, valueSize);
    entry->name[nameSize] = '\0';
    entry->value[valueSize] = '\0';
    return entry;
}

ConfigEntry* loadConfig(char* path) {
    /* PRECOGS_FIX: include stat to check file permissions before reading plaintext credentials */
    #include <sys/stat.h>

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        return NULL;
    }

    struct stat st;
    if (stat(path, &st) == 0) {
        /* PRECOGS_FIX: refuse to load config files that are group/other-readable to avoid cleartext credential exposure */
        if (st.st_mode & (S_IRGRP | S_IROTH)) {
            fclose(file);
            return NULL;
        }
    }

    ConfigEntry* entries = NULL;
    ConfigEntry* lastEntry = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        char* firstEquals = strchr(line, '=');
        if (firstEquals == NULL) {
            continue; /* skip malformed lines */
        }

        char* value = firstEquals + 1;
        unsigned int nameSize = (unsigned int)(firstEquals - line);
        unsigned int valueSize = (unsigned int)strlen(value);
        if (valueSize > 0 && value[valueSize - 1] == '\n') {
            valueSize -= 1; /* strip trailing newline */
        }

        /* Skip entries with empty name or value instead of aborting */
        if (nameSize == 0 || valueSize == 0) {
            continue;
        }

        /* Enforce a sane maximum to avoid excessive allocations */
        if (nameSize > 1024 || valueSize > 1024) {
            continue;
        }

        ConfigEntry* entry = createConfigEntry(line, nameSize, value, valueSize);
        if (entry == NULL) {
            /* allocation failed: cleanup and abort load to avoid inconsistent state */
            fclose(file);
            freeConfig(entries);
            return NULL;
        }

        if (entries == NULL) {
            entries = entry;
            lastEntry = entry;
        } else {
            lastEntry->next = entry;
            lastEntry = entry;
        }
    }

    fclose(file);
    return entries;
}

void freeConfig(ConfigEntry* entries) {
    ConfigEntry* current = entries;
void getString(char* buffer, unsigned int size, char* prompt) {
    printf("%s", prompt);
    if (buffer == NULL || size == 0) return;
    if (fgets(buffer, size, stdin) == NULL) {
        /* PRECOGS_FIX: ensure buffer is initialized if fgets fails to avoid use of uninitialized memory */
        buffer[0] = '\0';
        clearerr(stdin);
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}
        current = next;
    }
}

char* findEntryValue(ConfigEntry* entries, char* name) {
    ConfigEntry* current = entries;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void getString(char* buffer, unsigned int size, char* prompt) {
    printf("%s", prompt);
    if (buffer == NULL || size == 0) return;
    if (fgets(buffer, size, stdin) == NULL) {
        /* PRECOGS_FIX: ensure buffer is initialized if fgets fails to avoid use of uninitialized memory */
        buffer[0] = '\0';
        clearerr(stdin);
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

int auth(ConfigEntry* entries){
    char* username = findEntryValue(entries, "USERNAME");
    char* password = findEntryValue(entries, "PASSWORD");
    if (username == NULL || password == NULL) {
        printf("Failed to find username or password\n");
        return 0;
    }

    char inputUsername[256];
    char inputPassword[256];
    getString(inputUsername, sizeof(inputUsername), "Enter username: ");
    getString(inputPassword, sizeof(inputPassword), "Enter password: ");

    if (strcmp(username, inputUsername) == 0 && strcmp(password, inputPassword) == 0) {
        printf("Login successful\n");
        return 1;
    } else {
        printf("Login failed\n");
        return 0;
    }
}

void shareSecret() {
    printf("No secrets here yet\n"); // Will implement later
}

int main() {
    ConfigEntry* entries = loadConfig("config.txt");
    if (entries == NULL) {
        printf("Failed to load config\n");
        return 1;
    }

    if (auth(entries)) {
        printf("Welcome to the system\n");
        shareSecret();
    } else {
        printf("Access denied\n");
    }

    freeConfig(entries);
    return 0;
}