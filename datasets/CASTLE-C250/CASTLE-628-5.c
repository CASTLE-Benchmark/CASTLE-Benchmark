#include <stdio.h>
#include <ctype.h>

int hex_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else {
        return -1;
    }
}

void url_decode(const char *src, char *dest) {
    size_t out = 0;
    const size_t MAX_DEST = 1024; // PRECOGS_FIX: enforce an explicit maximum destination size to prevent overrun

    while (*src && out < MAX_DEST - 1) {
        if (*src == '%') {
            /* Ensure we have two more characters before indexing, and use unsigned char for isxdigit */
            if (src[1] != '\0' && src[2] != '\0' && isxdigit((unsigned char)src[1]) && isxdigit((unsigned char)src[2])) {
                int high = hex_to_int(src[1]);
                int low = hex_to_int(src[2]);
                if (high >= 0 && low >= 0) {
                    dest[out++] = (char)(high * 16 + low);
                } else {
                    /* Fallback: copy '%' if decoding failed */
                    dest[out++] = *src;
                }
                src += 3;
            } else {
                /* Invalid encoding or truncated percent-encoding: copy '%' as-is */
                dest[out++] = *src;
                src++;
            }
        } else if (*src == '+') {
            dest[out++] = ' ';
            src++;
        } else {
            dest[out++] = *src;
            src++;
        }
    }
    dest[out] = '\0';
}

int main() {
    /* PRECOGS_FIX: make the URL a writable array rather than a pointer to a string literal */
    char url[] = "http://localhost:8080/?search=%3C%3Cscript%3Escript%20%3Ealert(1)%3C/script%20%3C/script%3E%3E"; 
    char decodedUrl[1024] = {0};

    /* PRECOGS_FIX: pass arguments in correct order: src (url), dest (decodedUrl) */
    url_decode(url, decodedUrl);

    printf("Decoded URL: %s\n", decodedUrl);
    return 0;
}