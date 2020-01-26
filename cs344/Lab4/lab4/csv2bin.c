#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "file_struct.h"

int main(int argc, char* argv[]){
	#define MAX_BUFF 1024
	int opt;
	int v_flag = 0;
	int h_flag = 0;
	int unknown_flag = 0;
	//FILE* inputFile;
	FILE* fileIn = stdin;
	char* argInVal = NULL;
	char buff[MAX_BUFF];
	file_struct_t my_struct;
	int outputFile = STDOUT_FILENO;

	while((opt = getopt(argc, argv, "o:i:vh")) != -1){
		switch(opt){
			case 'i':
				fileIn = fopen(optarg,"r");
				if(fileIn == NULL){
                                        perror("Error: File Not Found");
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
                                outputFile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
                                if(outputFile < 0){
                                        perror("Error: no file\n");
                                        exit(3);
                                 }
                                break;
		}
	}
		char *ch;
		const char deli[] = {",\n"};
		//if(fileIn == NULL) {
		//	perror("Error: cannot open file\n");
		//	exit(2);
		//}
		//inputFile = fopen(argInVal, "r");
		//ch = fgets(buff, MAX_BUFF, inputFile);

		//ch = fgets(buff, MAX_BUFF, inputFile);
                ch = fgets(buff, MAX_BUFF, fileIn);

                ch = fgets(buff, MAX_BUFF, fileIn);
		while (ch != NULL) {
			char *token;
			memset(&my_struct, 0, sizeof(my_struct));


			token = strtok(buff, deli);
			strcpy(my_struct.id, token);
			//ch = fgets(buff, MAX_BUFF, inputFile);

			token = strtok(NULL, deli);
                        strcpy(my_struct.fname, token);

			token = strtok(NULL, deli);
			strcpy(my_struct.mname, token);

			token = strtok(NULL, deli);
                        strcpy(my_struct.lname, token);

			token = strtok(NULL, deli);
                        strcpy(my_struct.street, token);

			token = strtok(NULL, deli);
                        strcpy(my_struct.city, token);

			token = strtok(NULL, deli);
                        strcpy(my_struct.zip, token);

			token = strtok(NULL, deli);
                        strcpy(my_struct.country_code, token);

			token = strtok(NULL, deli);
                        strcpy(my_struct.email, token);

			token = strtok(NULL, deli);
                        strcpy(my_struct.phone, token);

			write(outputFile, &my_struct, sizeof(my_struct));
			//ch = fgets(buff, MAX_BUFF, inputFile);
			ch = fgets(buff, MAX_BUFF, fileIn);
		}
	if(v_flag != 0){
		fprintf(stderr, "\n%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
	}
	if(h_flag != 0){
		printf("./csv2bin\n");
		printf("\toptions: oiv\n");
		printf("\t-o: preceeds the name of the output file\n");
		printf("\t-i: preceeds the name of the input file\n");
		printf("\t-v: verbose stuff TODO\n");
		return(0);
	}
	if(unknown_flag != 0){
		fprintf(stderr, "Unknown option character(s) -%c\n", optopt);
		return(1);
	}
	fclose(fileIn);
	return(0);
}

