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

int compareId (const void * a, const void * b);

int compareId (const void * a, const void * b) {
	file_index_t *p1 = (file_index_t *)a;
	file_index_t *p2 = (file_index_t *)b;

	return strcmp(p1->id, p2->id);
}

int main(int argc, char *argv[])
{
  	int numIDs = -1;
	int i;
	int v_flag = 0;
	int h_flag = 0;
	int i_flag = 0;
	int big_i = 0;
	int unknown_flag = 0;

	int opt;
	int iFd = STDIN_FILENO;
	int oFd = STDOUT_FILENO;
	int lookupFd;

    	int nelm = 0;

	file_struct_t bin_info;
	file_index_t *ra = NULL;
        file_index_t* my_index = NULL;
	int bytes_read = -1;
	struct stat sb;
	int fileSize;
	file_struct_t my_struct;
	char binArg[MAX_BUFF];
	while((opt = getopt(argc, argv, "o:i:vI:h")) != -1){
			switch(opt){
				case 'i':
					i_flag = 1;
					strcpy(binArg, optarg);
                    			break;
				case 'v':
					v_flag = 1;
					break;
				case 'I':
					big_i = 1;
					lookupFd = open(optarg, O_RDONLY);
					//printf(" %d\n", __LINE__);
					break;
				case 'h':
					h_flag = 1;
					break;
				case '?':
					unknown_flag = 1;
					break;
				case 'o':
					oFd = open(optarg, O_CREAT | O_WRONLY);
					if(oFd < 0){
						perror("Error: File Not Found to write");
                	    			exit(3);
					}
                	break;
			}
	}

	if (i_flag == 0){
		printf("Missing mandatory input\n\n");
		exit(1);
	}
	//=====================BIG I FLAG ==========================================
	fstat(lookupFd, &sb);
	fileSize = sb.st_size/sizeof(file_index_t);
	numIDs = argc - optind;
	if(big_i == 1){
		//printf(" %d\n", __LINE__);
		if(lookupFd < 0){
			perror("Error: File Not Found to read");
			exit(3);
		}
		{
		  fstat(lookupFd, &sb);
		  {
		    ra = malloc(sb.st_size);
		    memset(ra, 0, sb.st_size);
		    read(lookupFd, ra, sb.st_size);
		  }
		}
		iFd = open(binArg, O_RDONLY);
		nelm++;
		//printf(" %d  %d\n", __LINE__, iFd);
		bytes_read = read(iFd, &my_struct, sizeof(file_struct_t));
		//printf(" %d\n", __LINE__);
		for(i = 0; i < numIDs; i++){
		  file_index_t tmp;

		  strcpy(tmp.id, argv[optind]);
		  tmp.offset = 1;
			my_index = bsearch(&tmp, ra, fileSize, sizeof(file_index_t), compareId);
			//printf(" %d\n", __LINE__);
			if (my_index != NULL) {
				lseek(iFd, my_index->offset, SEEK_SET);
				bytes_read = read(iFd, &bin_info, sizeof(file_struct_t));
				dprintf(oFd, "ID: %s\n\tFName\t\t: %s\n\tMName\t\t: "
					"%s\n\tLName\t\t: %s\n\tStreet\t\t: %s\n\tCity\t\t: "
					"%s\n\tZip\t\t: %s\n\tCountry Code\t: %s\n\tEMail\t\t: %s\n\tPhone\t\t: "
					"%s\n", bin_info.id, bin_info.fname, bin_info.mname
					, bin_info.lname, bin_info.street, bin_info.city, bin_info.zip, bin_info.country_code, bin_info.email, bin_info.phone);
			}
			else {
			  printf("not found\n");
			}
			optind++;
		}
	}


	//==================== NO BIG I FLAG =======================================
	else{
	  	//int reader;
		//reader = -1;
		int notFounds = 0;
		while(numIDs > 0){
			iFd = open(binArg, O_RDONLY);
                        if(iFd == -1){
                        	perror("Error: File Not Found to read");
                                exit(3);
                        }
			memset(&bin_info, 0, sizeof(file_struct_t));
			while((bytes_read = read(iFd, &bin_info, sizeof(file_struct_t))) > 0){
				if(strcmp(bin_info.id,argv[optind]) == 0){
					//printf("searching for: %s\n", argv[optind]);
					dprintf(oFd, "ID: %s\n\tFName\t\t: %s\n\tMName\t\t: %s\n\tLName\t\t: %s\n\t"
						"Street\t\t: %s\n\tCity\t\t: %s\n\tZip\t\t: %s\n\t"
						"Country Code\t: %s\n\tEMail\t\t: %s\n\tPhone\t\t: "
						"%s\n", bin_info.id,bin_info.fname, bin_info.mname, bin_info.lname, bin_info.street, bin_info.city, bin_info.zip, bin_info.country_code, bin_info.email, bin_info.phone);
					optind++;
					break;
				}
				memset(&bin_info, 0, sizeof(file_struct_t));
			}
			close(iFd);
			numIDs--;
		}
		if(notFounds > 0){
			printf("%i ID(s) not found", notFounds);
		}
	}
	// ================== HELP VERBOSE and UNKNOWN FLAGS ====================

	if(v_flag != 0){
	fprintf(stderr, "\n%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	}
	if(h_flag != 0){
		printf("./csv2bin\n");
		printf("\toptions: oivhI\n");
		printf("\t-o: preceeds the name of the output file\n");
		printf("\t-i: preceeds the name of the input file and is MANDATORY\n");
		printf("\t-I: preceeds the name of the index file used for lookup. Speeds things up but is NOT mandatory\n");
		printf("\t-v: verbose stuff\n");
		return(0);
	}
	if(unknown_flag != 0){
		fprintf(stderr, "Unknown option character(s) -%c\n", optopt);
		return(1);
	}

	return 0;

}
