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
	FILE* inputFile;
	FILE* outputFile = stdout;
	ssize_t bytes_read = -1;
        file_struct_t my_struct;
        int iFd = STDIN_FILENO;
	char buff[MAX_BUFF];
	char* ch;
        char* token;
        const char deli[] = {",\n"};
	file_struct_t my_temp_struct;

	while((opt = getopt(argc, argv, "o:i:vh")) != -1){
                switch(opt){
                        case 'i':
				iFd = open(optarg, O_RDONLY);
				if(iFd < 0){
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
				outputFile = fopen(optarg, "w");
				if(outputFile == NULL){
					perror("Error: File Not Found");
                                        exit(3);
				}
                                break;
                }
        }
	{
		//PULL JUST HEADERS FROM CSV
		memset(&my_struct, 0, sizeof(my_struct));
		inputFile = fopen("SampleData.csv", "r"); //use static open path bc headers don't change and we can
		ch = fgets(buff, MAX_BUFF, inputFile);
		token = strtok(buff, deli);
                strcpy(my_struct.id, token);

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
		//FPRINTF ORIGINAL HEADERS INTO NEW CSV
			fprintf(outputFile, "%s,", my_struct.id);
                        fprintf(outputFile, "%s,", my_struct.fname);
                        fprintf(outputFile, "%s,", my_struct.mname);
                        fprintf(outputFile, "%s,", my_struct.lname);
                        fprintf(outputFile, "%s,", my_struct.street);
                        fprintf(outputFile, "%s,", my_struct.city);
                        fprintf(outputFile, "%s,", my_struct.zip);
                        fprintf(outputFile, "%s,", my_struct.country_code);
                        fprintf(outputFile, "%s,", my_struct.email);
                        fprintf(outputFile, "%s\n", my_struct.phone);
		//
		memset(&my_struct, 0, sizeof(file_struct_t));
		while ((bytes_read = read(iFd, &my_temp_struct, sizeof(file_struct_t))) > 0) {
			memset(&my_struct, 0, sizeof(my_temp_struct));
			fprintf(outputFile, "%s,", my_temp_struct.id);
			fprintf(outputFile, "%s,", my_temp_struct.fname);
			fprintf(outputFile, "%s,", my_temp_struct.mname);
			fprintf(outputFile, "%s,", my_temp_struct.lname);
			fprintf(outputFile, "%s,", my_temp_struct.street);
			fprintf(outputFile, "%s,", my_temp_struct.city);
			fprintf(outputFile, "%s,", my_temp_struct.zip);
			fprintf(outputFile, "%s,", my_temp_struct.country_code);
			fprintf(outputFile, "%s,", my_temp_struct.email);
			fprintf(outputFile, "%s\n", my_temp_struct.phone);
		}
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

	close(iFd);
	fclose(inputFile);
	fclose(outputFile);
	return(0);
}
