#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h> 

#include "tests.h"

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    int ids[] = {10,11,12};
    char *names[] = {"Moe", "Larry", "Curley"};
    int ages[] = {45,55,65};
    char *addrs[] = {"Oregon", "Washington","Idaho"};
    float sals[] = {1000.7, 7500.51, 99.99};


    /* Open database */
    rc = sqlite3_open(DB_NAME, &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(1);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = "insert into company (id,name,age,address,salary) " 
        "values (?1, ?2, ?3, ?4, ?5 ); "  
        ;

    {
        sqlite3_stmt *stmt;
        int i;
        int res;

        sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        for (i = 0; i < 3; i++) {
            sqlite3_bind_int(stmt, 1, ids[i]);
            sqlite3_bind_text(stmt, 2, names[i], strlen(names[i]), SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, ages[i]);
            sqlite3_bind_text(stmt, 4, addrs[i], strlen(addrs[i]), SQLITE_STATIC);
            sqlite3_bind_double(stmt, 5, sals[i]);

            res = sqlite3_step(stmt);
            if (res == SQLITE_DONE) {
                sqlite3_reset(stmt);
            }
            else {
                fprintf(stderr, "error on %d\n", i);
                break;
            }
        }

        sqlite3_finalize(stmt);
    }
   
   if( rc != SQLITE_OK ) {
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } 
   else {
       fprintf(stdout, "Records inserted successfully\n");
   }
   sqlite3_close(db);
   return 0;
}
