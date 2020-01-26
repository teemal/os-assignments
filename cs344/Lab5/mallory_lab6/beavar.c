#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

#include "beavar.h"

#define MAXBUFF 1024

#define MODE_UNKNOWN 0
#define MODE_APPEND 1
#define MODE_EXTRACT 2
#define MODE_LITTLE_T 3
#define MODE_BIG_T 4

void helpInfo(void);
void helpInfo(void) {
	printf("a | Add member(s) from the command line to the archive file.\n");
	printf("e | Extract members from archive to file, from command line.\n");
	printf("h | Show the help text\n");
	printf("t | Short table of contents.\n");
	printf("T | Long table of contents.\n");
	printf("v | Verbose processing.\n");
	printf("V | Print the version information and exit.\n");
}

int main(int argc, char *argv[]) {
  int i;
	int opt;
	int oFd = STDIN_FILENO; //the beavar file
	int oFd2 = STDIN_FILENO; //member file
	ssize_t bytes_read = -1;
	int unknownFlag = 0, a_flag = 0, e_flag = 0, t_flag = 0, T_flag = 0,
		v_flag = 0, h_flag = 0;
	struct stat my_stat;
	beavar_hdr_t my_struct;
	int bMode = MODE_UNKNOWN;
	char *beavarFileName = NULL;
	while ((opt = getopt(argc, argv, "a:e:ht:T:vV")) != -1) {
		switch(opt){
			case 'a':
			  if (bMode != MODE_UNKNOWN) {
			    perror("invalid command");
			    exit(EXIT_FAILURE);
			  }
			  bMode = MODE_APPEND;
			  beavarFileName = optarg;
				a_flag = 1;
				break;
			case 'e':
				 if (bMode != MODE_UNKNOWN) {
                            perror("invalid command");
                            exit(EXIT_FAILURE);
                          }
                          bMode = MODE_EXTRACT;
                          beavarFileName = optarg;

				e_flag = 1;
				break;
			case 'h':
				h_flag = 1;
				helpInfo();
				break;
			case 't':
				if(bMode != MODE_UNKNOWN) {
				perror("invalid command");
					exit(EXIT_FAILURE);
				}
				bMode = MODE_LITTLE_T;
				beavarFileName = optarg;
				t_flag = 1;
				break;
			case 'T':
				if(bMode != MODE_UNKNOWN) {
                                perror("invalid command");
					exit(EXIT_FAILURE);
                                }
                                bMode = MODE_BIG_T;
				beavarFileName = optarg;
				T_flag = 1;
				break;
			case 'v':
				v_flag = 1;
				break;
			case 'V':
				printf("Beavar Version 1.0\n");
				exit(1);
				break;
			case '?':
				unknownFlag = 1;
				break;
		}
	}

	if(bMode == MODE_APPEND) {
		oFd = open(beavarFileName, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (oFd < 0) {
			// when file exists
			char buffer[BEAVAR_ID_LEN + 1] = {'\0'};
			oFd = open(beavarFileName, O_RDWR | O_APPEND);
			read(oFd, buffer, BEAVAR_ID_LEN);
			if (strcmp(buffer, BEAVAR_ID) != 0) {
                                perror("ERROR: File not a BEAVAR file\n");
                                exit(3);
                        }
	  	}
		else {
			// file is created and header added
			char buffer[BEAVAR_ID_LEN] = {BEAVAR_ID};
			write(oFd, buffer, BEAVAR_ID_LEN);
		}

	  //opening the member file(s)
	for(i = optind; i < argc; i++) {
		char buffy[1000];
		long int adate;
		long int mdate;
		int uid;
		int gid;
		int size;
		int len;

		oFd2 = open(argv[i], O_RDONLY);

		if(oFd2 < 0) {
			perror("No member file exsist or given");
			exit(BEAVAR_FILE_COULD_NOT_OPEN);
		}
		// stat the file
		fstat(oFd2, &my_stat);

		// fill beavar_struct_t
		adate = my_stat.st_atime;
		mdate = my_stat.st_mtime;
		uid = my_stat.st_uid;
		gid = my_stat.st_gid;
		size = my_stat.st_size;

		memset(&my_struct, ' ', sizeof(my_struct));

		memcpy(my_struct.beavar_fname, argv[i],strlen(argv[i]));

		len = strlen(argv[i]);
		sprintf(my_struct.beavar_fname_len,"%2d", len);

		sprintf(my_struct.beavar_adate, "%10ld", adate);

		sprintf(my_struct.beavar_mdate, "%10ld", mdate);

		sprintf(my_struct.beavar_uid, "%6d", uid);

		sprintf(my_struct.beavar_gid, "%6d", gid);

		sprintf(my_struct.beavar_mode, "%7o", my_stat.st_mode);

		sprintf(my_struct.beavar_size, "%12d", size);

		strcpy(my_struct.beavar_hdr_end, BEAVAR_HDR_END);

		// write beavar_start_t to beavar file
		write(oFd, &my_struct, sizeof(my_struct));

		//will be used to read into the member file to extract its info
		while((bytes_read = read(oFd2, buffy, sizeof(buffy))) > 0) {

			//writes the contents of the member file
			write(oFd, buffy, bytes_read);
		}
		close(oFd2);

	}//end of for loop

	}//end of MODE_APPEND

	if(bMode == MODE_EXTRACT){

		printf("hello\n");
	}


	if(bMode == MODE_LITTLE_T) {
		char buffer[BEAVAR_ID_LEN + 1] = {'\0'};
		char nameBuff[BEAVAR_MAX_FILE_NAME_LEN + 1] = {'0'};
		char buffy[BEAVAR_FILE_SIZE + 1] = {'0'};
		char lenBuff[BEAVAR_FNAME_LEN_SIZE + 1] = {'0'};

		int size;
		int lenSize;

		//open beavar file as read only and check of a file is givien
		oFd = open(beavarFileName, O_RDONLY);
		if(oFd < 0) {
			perror("No file exsist");
			exit(BEAVAR_FILE_COULD_NOT_OPEN);
		}
		//check if it has a header
		read(oFd, buffer, BEAVAR_ID_LEN);
		if (strcmp(buffer, BEAVAR_ID) != 0) {
			perror("ERROR: File not a BEAVAR file\n");
			exit(BEAVAR_FILE_MEMBER_BAD);
		}

		printf("Short table of contents for beavar archive file: %s\n", beavarFileName);
		//read through th beavar file
		while((bytes_read = read(oFd, &my_struct, sizeof(my_struct))) > 0) {

			memset(nameBuff, 0, sizeof(nameBuff));
			memset(buffy, 0, sizeof(buffy));

			memcpy(buffy, my_struct.beavar_size, BEAVAR_FILE_SIZE);
			memcpy(lenBuff,my_struct.beavar_fname_len, BEAVAR_FNAME_LEN_SIZE);

			lenSize = atoi(lenBuff);
			size = atoi(buffy);

			memcpy(nameBuff, my_struct.beavar_fname, lenSize);

			lseek(oFd,size,SEEK_CUR);

			printf("%s\n", nameBuff);
		}
	}

        if(bMode == MODE_BIG_T) {
		char buffer[BEAVAR_ID_LEN + 1] = {'\0'};
                char nameBuff[BEAVAR_MAX_FILE_NAME_LEN + 1] = {'\0'};
                char buffy[BEAVAR_FILE_SIZE + 1] = {'\0'};
                char lenBuff[BEAVAR_FNAME_LEN_SIZE + 1] = {'\0'};
		char modeBuff[BEAVAR_MODE_SIZE + 1] = {'\0'};
		char pbuff[BEAVAR_UGID_SIZE + 1] = {'\0'};
		char gbuff[BEAVAR_UGID_SIZE + 1] = {'\0'};
		char atbuff[BEAVAR_DATE_SIZE + 1] = {'\0'};
		char mtbuff[BEAVAR_DATE_SIZE + 1] = {'\0'};
		char buf1[1024];
		char buf2[1024];

                int size;
                int lenSize;
		char *ptr;
		long string_to_octal;
		long pass;
		long gp;
		long atm;
		long mtm;

		struct group *grp;
		struct passwd *pws;
		struct tm *tm;

		//open beavar file as read only and check of a file is givien
                oFd = open(beavarFileName, O_RDONLY);
                if(oFd < 0) {
                        perror("No file exsist");
                        exit(BEAVAR_FILE_COULD_NOT_OPEN);
                }
                //check if it has a header
                read(oFd, buffer, BEAVAR_ID_LEN);
                if (strcmp(buffer, BEAVAR_ID) != 0) {
                        perror("ERROR: File not a BEAVAR file\n");
                        exit(BEAVAR_FILE_MEMBER_BAD);
                }
		printf("Long Table of Contents for beavar archive file: %s\n", beavarFileName);
		while((bytes_read = read(oFd, &my_struct, sizeof(my_struct))) > 0) {
			memset(nameBuff, 0, sizeof(nameBuff));
                        memset(buffy, 0, sizeof(buffy));
			memset(modeBuff, 0, sizeof(modeBuff));
			memset(pbuff, 0, sizeof(pbuff));
			memset(gbuff, 0, sizeof(gbuff));
			memset(atbuff, 0, sizeof(atbuff));
			memset(mtbuff, 0, sizeof(mtbuff));

                        memcpy(buffy, my_struct.beavar_size, BEAVAR_FILE_SIZE);
                        memcpy(lenBuff,my_struct.beavar_fname_len, BEAVAR_FNAME_LEN_SIZE);

                        lenSize = atoi(lenBuff);
                        size = atoi(buffy);

                        memcpy(nameBuff, my_struct.beavar_fname, lenSize);
			memcpy(modeBuff,  my_struct.beavar_mode,BEAVAR_MODE_SIZE);
			memcpy(pbuff, my_struct.beavar_uid, BEAVAR_UGID_SIZE);
			memcpy(gbuff, my_struct.beavar_gid, BEAVAR_UGID_SIZE);
			memcpy(atbuff, my_struct.beavar_adate, BEAVAR_DATE_SIZE);
			memcpy(mtbuff, my_struct.beavar_mdate, BEAVAR_DATE_SIZE);

                        lseek(oFd,size,SEEK_CUR);

                        printf("    File name:%s\n", nameBuff);
			printf("File size:   %d bytes\n", size);

			printf("Permissions: ");
			string_to_octal = strtol(modeBuff, &ptr, 8);
			printf( (S_ISDIR(string_to_octal)) ? "d" : "-");
  			printf( (string_to_octal & S_IRUSR) ? "r" : "-");
  			printf( (string_to_octal & S_IWUSR) ? "w" : "-");
  			printf( (string_to_octal & S_IXUSR) ? "x" : "-");
  			printf( (string_to_octal & S_IRGRP) ? "r" : "-");
  			printf( (string_to_octal & S_IWGRP) ? "w" : "-");
  			printf( (string_to_octal & S_IXGRP) ? "x" : "-");
  			printf( (string_to_octal & S_IROTH) ? "r" : "-");
  			printf( (string_to_octal & S_IWOTH) ? "w" : "-");
  			printf( (string_to_octal & S_IXOTH) ? "x" : "-");
			printf("       (%s)\n", &modeBuff[3]);

			pass = strtol(pbuff, &ptr, 10);
			pws = getpwuid(pass);
			printf("File owner:  %-8.8s         (uid:%s)\n",pws->pw_name, pbuff);

			gp = strtol(gbuff, &ptr, 10);
			grp = getgrgid(gp);
			printf("File group:  %-8.8s         (gid:%s)\n",grp->gr_name, gbuff);

			atm = strtol(atbuff, &ptr, 10);
			tm = localtime(&atm);
			strftime(buf1,sizeof(buf1),"%Y-%m-%d %H:%M:%S %z (%Z) %a", tm);
			printf("Access date: %s %s\n",buf1, atbuff);

			mtm = strtol(mtbuff, &ptr, 10);
			tm = localtime(&mtm);
			strftime(buf2,sizeof(buf2),"%Y-%m-%d %H:%M:%S %z (%Z) %a", tm);
			printf("Modify date: %s %s\n",buf2, mtbuff);
		}
        }

	if(a_flag == 0 && e_flag == 0 && t_flag == 0 && T_flag == 0 && h_flag == 0) {
		printf("error: No Command(s) Were Given\n");
		exit(1);
	}

	if (unknownFlag == 1) {
		fprintf(stderr, "Unknown option character(s) - %c\n", optopt);
		return(1);
	}

	if(v_flag == 1) {
		fprintf(stderr, "%s %s %u\n",__FILE__, __FUNCTION__, __LINE__);
	}
	close(oFd);
	close(oFd2);
	return 0;
}

