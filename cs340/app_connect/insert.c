#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

#include "tests.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;

    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
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
        "values (1, 'Paul', 32, 'California', 20000.00 ); "  
        "insert into company (id,name,age,address,salary) "  
        "values (2, 'Allen', 25, 'Texas', 15000.00 ); "     
        "insert into company (id,name,age,address,salary)" 
        "values (3, 'Teddy', 23, 'Norway', 20000.00 );" 
        "insert into company (id,name,age,address,salary)" 
        "values (4, 'Mark', 25, 'Richmond ', 65000.00 );"
        ;

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
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
