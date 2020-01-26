#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

#include "tests.h"

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    /* Open database */
    rc = sqlite3_open(DB_NAME, &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(1);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    sql = "update company set salary = 17000.00 where id = ?1;";

    {
        sqlite3_stmt *stmt;

        sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        sqlite3_bind_int(stmt, 1, 3);

        sqlite3_step(stmt);

        sqlite3_finalize(stmt);
    }
   
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
