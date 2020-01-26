#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

#include "tests.h"

#define NO_CALLBACK
#ifndef NO_CALLBACK
static int newCallback = 0;

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
   
    if (newCallback) {
        for(i = 0; i < argc; i++) {
            printf("%-15s", azColName[i]);
        }
        printf("\n");
        newCallback = 0;
    }
    for(i = 0; i < argc; i++) {
        printf("%-15s", argv[i] ? argv[i] : "NULL");
    }
   
    printf("\n");
    return 0;
}
#endif // NO_CALLBACK

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
#ifndef NO_CALLBACK
    const char* data = "Callback function called";
#endif // NO_CALLBACK

    /* Open database */
    rc = sqlite3_open(DB_NAME, &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(1);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = "select * from company;";

#ifdef NO_CALLBACK
    {
        sqlite3_stmt *stmt;
        int row_count = 0;

        sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            int i;
            int num_cols = sqlite3_column_count(stmt);

            if (row_count == 0) {
                for (i = 0; i < num_cols; i++) {
                    printf("%-15s", sqlite3_column_name(stmt, i));
                }
                printf("\n");
            }            
            for (i = 0; i < num_cols; i++) {
                switch (sqlite3_column_type(stmt, i)) {
                case (SQLITE3_TEXT):
                    printf("%-15s", sqlite3_column_text(stmt, i));
                    break;
                case (SQLITE_INTEGER):
                    printf("%-15d", sqlite3_column_int(stmt, i));
                    break;
                case (SQLITE_FLOAT):
                    printf("%-15.2f", sqlite3_column_double(stmt, i));
                    break;
                default:
                    break;
                }
            }
            printf("\n");
            row_count ++;
        }
        sqlite3_finalize(stmt);
    }
#else // NO_CALLBACK

    newCallback = 1;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
#endif // NO_CALLBACK

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
    return 0;
}
