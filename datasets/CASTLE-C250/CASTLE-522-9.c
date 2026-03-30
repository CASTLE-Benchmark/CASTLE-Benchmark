#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Credentials
    const char *server = getenv("DB_ADDRESS");
    const char *user = getenv("DB_USERNAME");
    const char *password = getenv("DB_PASSWORD");
    const char *database = getenv("DB_DATABASE");
    if (server == NULL || user == NULL || password == NULL || database == NULL) {
        printf("Missing database credentials\n");
        return 1;
    }

    // Initialize MySQL connection
    conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    // Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "Connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    /* Enforce that the connection uses TLS; fail closed if not. */
    const char *cipher = mysql_get_ssl_cipher(conn);
    if (cipher == NULL) {
        fprintf(stderr, "Connection is not encrypted (no SSL/TLS). Aborting.\n");
        mysql_close(conn);
        return 1; // PRECOGS_FIX: enforce TLS by aborting when no TLS cipher negotiated
    }

    // Send SQL query
    if (mysql_query(conn, "SELECT id, token FROM tokens")) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    // Store the result
    res = mysql_store_result(conn);
    if (res == NULL) {
        /* mysql_store_result() can return NULL if the query produced no result set
           or on error. Distinguish both cases. */
        if (mysql_field_count(conn) == 0) {
            // Query returned no data (not a SELECT). Nothing to print.
            mysql_close(conn);
            return 0;
        } else {
            fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
            mysql_close(conn);
            return 1; // PRECOGS_FIX: validate mysql_store_result() before using it to avoid NULL deref
        }
    }

    // Determine fields and find index of sensitive column 'token'
    int num_fields = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_fields(res);
    int token_idx = -1;
    for (int i = 0; i < num_fields; i++) {
        if (fields[i].name && strcmp(fields[i].name, "token") == 0) {
            token_idx = i;
            break;
        }
    }

    // Output the result row by row, redacting sensitive columns
    while ((row = mysql_fetch_row(res))) {
        unsigned long *lengths = mysql_fetch_lengths(res);
        for (int i = 0; i < num_fields; i++) {
            if (row[i] == NULL) {
                printf("[NULL] ");
            } else if (i == token_idx) {
                // REDACT tokens to avoid leaking secrets
                printf("[REDACTED] ");
            } else {
                printf("[%.*s] ", (int) lengths[i], row[i]);
            }
        }
        printf("\n");
    }

    // Clean up
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}

            fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
            mysql_close(conn);
            return 1; // PRECOGS_FIX: validate mysql_store_result() before using it to avoid NULL deref
        }
    }

    // Determine fields and find index of sensitive column 'token'
    int num_fields = mysql_num_fields(res);
    MYSQL_FIELD *fields = mysql_fetch_fields(res);
    int token_idx = -1;
    for (int i = 0; i < num_fields; i++) {
        if (fields[i].name && strcmp(fields[i].name, "token") == 0) {
            token_idx = i;
            break;
        }
    }

    // Output the result row by row, redacting sensitive columns
    while ((row = mysql_fetch_row(res))) {
        unsigned long *lengths = mysql_fetch_lengths(res);
        for (int i = 0; i < num_fields; i++) {
            if (row[i] == NULL) {
                printf("[NULL] ");
            } else if (i == token_idx) {
                // REDACT tokens to avoid leaking secrets
                printf("[REDACTED] ");
            } else {
                printf("[%.*s] ", (int) lengths[i], row[i]);
            }
        }
        printf("\n");
    }

    // Clean up
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}
