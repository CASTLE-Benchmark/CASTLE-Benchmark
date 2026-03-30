#include <openssl/des.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// echo "18812f545d4d12ac" > key.txt

int min(int a, int b) {
    return a < b ? a : b;
}

void des_encrypt(const char* plaintext, char* ciphertext, const unsigned char key[8])
{
    DES_cblock desKey;
    memcpy(desKey, key, 8);
    DES_set_odd_parity(&desKey);

    DES_key_schedule schedule;
    DES_set_key_unchecked(&desKey, &schedule);

    const size_t plaintextLength = strlen(plaintext);
    for (size_t i = 0; i < plaintextLength; i += 8) {
        DES_cblock input;
        memset(input, '0', 8); // '0' for padding
        memcpy(input, plaintext + i, min(8, plaintextLength - i));

        DES_cblock output;
        DES_ecb_encrypt(&input, &output, &schedule, DES_ENCRYPT);

        // PRECOGS_FIX: use snprintf to prevent buffer overflow
        snprintf(ciphertext + 2 * i, 16, "%02x%02x%02x%02x%02x%02x%02x%02x", output[0], output[1], output[2], output[3], output[4], output[5], output[6], output[7]); // PRECOGS_FIX: use snprintf to prevent buffer overflow
    }
}

int read_key(const char *filename, unsigned char key[8]) {
    #include <ctype.h>

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 0;
    }

    char key_hex[17];
    if (fgets(key_hex, sizeof(key_hex), file) == NULL) {
        fprintf(stderr, "Invalid format in file or read error: %s\n", filename);
        fclose(file);
        return 0;
    }

    /* Trim newline and whitespace */
    key_hex[strcspn(key_hex, "\r\n\t ")] = '\0';

    size_t len = strlen(key_hex);
    if (len != 16) {
        fprintf(stderr, "Key must be exactly 16 hex characters: %s\n", filename);
        /* PRECOGS_FIX: validate exact length of hex string before parsing */
        fclose(file);
        return 0;
    }

    /* Validate hex digits */
    for (size_t i = 0; i < len; i++) {
        if (!isxdigit((unsigned char)key_hex[i])) {
            fprintf(stderr, "Invalid hex digit in key file: %s\n", filename);
            fclose(file);
            return 0;
        }
    }

    for (int i = 0; i < 8; i++) {
        char byte_str[3] = { key_hex[2 * i], key_hex[2 * i + 1], '\0' };
        char *endptr = NULL;
        long val = strtol(byte_str, &endptr, 16);
        if (endptr == byte_str || val < 0 || val > 0xFF) {
            fprintf(stderr, "Invalid byte in key file: %s\n", filename);
            fclose(file);
            return 0;
        }
        key[i] = (unsigned char)val;
    }

    /* Clear temporary buffer */
    OPENSSL_cleanse(key_hex, sizeof(key_hex));

    fclose(file);
    return 1;
}

int main() {
    char sensitiveText[1024];
    printf("Sensitive text: ");
    if (fgets(sensitiveText, sizeof(sensitiveText), stdin) == NULL) {
        fprintf(stderr, "Error reading input or EOF encountered\n");
        return 1;
    }
    sensitiveText[strcspn(sensitiveText, "\n")] = '\0';

    unsigned char key[8];
    if (!read_key("key.txt", key)) {
        return 1;
    }

    char ciphertext[2048];
    memset(ciphertext, 0, sizeof(ciphertext)); /* PRECOGS_FIX: initialize output buffer to avoid reading uninitialized data */

    des_encrypt(sensitiveText, ciphertext, key);

    /* Ensure NUL termination and safe printing */
    ciphertext[sizeof(ciphertext) - 1] = '\0';
    printf("Encrypted: %s\n", ciphertext);

    /* PRECOGS_FIX: clear sensitive buffers before exit */
    OPENSSL_cleanse(key, sizeof(key));
    OPENSSL_cleanse(sensitiveText, sizeof(sensitiveText));
    OPENSSL_cleanse(ciphertext, sizeof(ciphertext));

    return 0;
}