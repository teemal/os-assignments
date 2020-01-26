#include <stdio.h>
#include <sqlite3.h> 

#include "tests.h"

int main(int argc, char* argv[])
{
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(DB_NAME, &db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(1);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);

    return(0);
}
