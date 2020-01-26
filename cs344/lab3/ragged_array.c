#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char const *argv[])
{
    char **ra = NULL;
    unsigned nelm = 0;
    FILE *iFile = stdin;
    char buffer[MAX_LINE_LENGTH];
    char *b_ptr = NULL;

    b_ptr = fgets(buffer, MAX_LINE_LENGTH, iFile);
    while(b_ptr != NULL){
        ra = realloc(ra, (nelm + 1) * sizeof(char*));
        ra[nelm++] = strdup(buffer);
        b_ptr = fgets(buffer, MAX_LINE_LENGTH, iFile);
    }
    int i;
    for (i = 0; i < nelm; i++) {
        printf("%04d:  %s", i+1, ra[i] );
	free(ra[i]);
    }
    free(ra);
    printf("\n");
    return(EXIT_SUCCESS);
}
