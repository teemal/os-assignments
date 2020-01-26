#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "file_struct.h"
#include "file_index.h"

#define MAX_BUFF 1024

int cmpfunc (const void * a, const void * b);

int cmpfunc (const void * a, const void * b) {
	file_index_t *p1 = (file_index_t *)a;
	file_index_t *p2 = (file_index_t *)b;

	return strcmp(p1->id, p2->id);
}

int main(int argc, char *argv[])
{
	file_struct_t my_struct;
	int bytes_read = -1;
	int j = 0;
	//int k = 0;
	int v_flag = 0;
	int h_flag = 0;
	int unknown_flag = 0;

	int opt;
	int iFd = STDIN_FILENO;
	int oFd = STDOUT_FILENO;

	file_index_t *ra = NULL;
    	int nelm = 0;

	while((opt = getopt(argc, argv, "o:i:vh")) != -1){
			switch(opt){
				case 'i':
					iFd = open(optarg, O_RDONLY);
					if(iFd < 0){
						perror("Error: File Not Found to read");
						exit(3);
					}
                    break;
				case 'v':
					v_flag = 1;
					break;
				case 'h':
					h_flag = 1;
					break;
				case '?':
					unknown_flag = 1;
					break;
				case 'o':
					oFd = open(optarg,O_CREAT | O_WRONLY);
					if(oFd < 0){
						perror("Error: File Not Found to write");
                	    exit(3);
					}
                	break;
			}
	}

	memset(&my_struct, 0, sizeof(file_struct_t));
	bytes_read = read(iFd, &my_struct, sizeof(file_struct_t));
	while (bytes_read > 0) {
		ra = realloc(ra, (nelm + 1) * sizeof(file_index_t));
		memset(&(ra[nelm]), 0, sizeof(file_index_t));
		strcpy(ra[nelm].id, my_struct.id);
		ra[nelm].offset = (nelm * sizeof(file_struct_t));


		nelm++;
		bytes_read = read(iFd, &my_struct, sizeof(file_struct_t));
	}
	//======================================================================
	qsort(ra, nelm, sizeof(file_index_t), cmpfunc);

	// ===========================print to file=============================
	for(j = 0; j < nelm; j++){
		write(oFd, &ra[j], sizeof(ra[j]));
	}
	// ================== HELP VERBOSE and UNKNOWN FLAGS ====================

	if(v_flag != 0){
	fprintf(stderr, "\n%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	}
	if(h_flag != 0){
		printf("./csv2bin\n");
		printf("\toptions: oiv\n");
		printf("\t-o: preceeds the name of the output file\n");
		printf("\t-i: preceeds the name of the input file\n");
		printf("\t-v: verbose stuff\n");
		return(0);
	}
	if(unknown_flag != 0){
		fprintf(stderr, "Unknown option character(s) -%c\n", optopt);
		return(1);
	}


	free(ra);
	close(oFd);
	close(iFd);
	return 0;
}
