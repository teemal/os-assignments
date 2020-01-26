#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

#include "tests.h"

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

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char* data = "Callback function called";

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

    newCallback = 1;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);

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
