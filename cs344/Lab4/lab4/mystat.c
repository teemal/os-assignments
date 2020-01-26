#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int file;


int main(int argc, char* argv[]){
    	int i;
	struct stat fileStat;
	struct tm* time;
	char buf[1024];
	struct group* group_name;
	struct passwd* password;

	if(argc < 2)
	return 1;

    	for(i = 1; i < argc; i++){
		if(lstat(argv[i],&fileStat) == -1){
			perror("Error: ");
			exit(EXIT_FAILURE);
		}
		printf("File: %s\n",argv[i]);
		printf("\tFile Type: ");
		switch(fileStat.st_mode & S_IFMT){
			case S_IFBLK: printf("block device\n");	break;
			case S_IFDIR: printf("directory\n"); break;
			case S_IFLNK:
			{
                        	 struct stat sl;
                                 char buff[1024];
                                 memset(buff, 0, 1024);
                                 if (stat(argv[i], &sl) == -1) {
                                        perror("Error: ");
                                        exit(EXIT_FAILURE);
                                 }
                                 readlink(argv[i], buff, 1024);
                                 printf("Symbolic link -> %s\n",buff);
                         }
			break;
			case S_IFIFO: printf("FIFO/pipe\n"); break;
			case S_IFREG: printf("regular file\n"); break;
			case S_IFSOCK: printf("socket\n"); break;
			default: printf("unknown file type!!!");
		}
		printf("\tDevice ID Number: %ld\n", fileStat.st_dev);
		printf("\tInode Number: %ld\n", fileStat.st_ino);
		printf("\tMode:  ");
   		printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    		printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
   		printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    		printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    		printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    		printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    		printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    		printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    		printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
		printf("\tOctal: %lo\n", (unsigned long)fileStat.st_mode);
		printf("\tLink Count: %ld\n", fileStat.st_nlink);
		if((password = getpwuid(fileStat.st_uid)) != NULL) {
                        printf("\towner ID:\t\t\t%-8.8s\t(UID = %d)\n", password->pw_name, fileStat.st_uid);
                }

		if ((group_name = getgrgid(fileStat.st_gid)) != NULL) {
                        printf("\tgroup ID:\t\t\t%-8.8s\t(GID = %d)\n", group_name->gr_name, fileStat.st_gid);
                }
		printf("\tPreffered IO Block Size: %ld Bytes \n", fileStat.st_blksize);
		printf("\tFile Size: %ld\n", fileStat.st_size);
		printf("\tBlocks Allocated:\t%ld\n",fileStat.st_blocks);
		printf("\tLast File Access:\t%ld (seconds since the epoch)\n", fileStat.st_atime);
		printf("\tLast File Modification:\t%ld (seconds since the epoch)\n", fileStat.st_mtime);
		printf("\tLast Status Change:\t%ld (seconds since the epoch)\n", fileStat.st_ctime);

                time = localtime(&fileStat.st_atime);
                strftime(buf,sizeof(buf), "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", time);
                printf("\tlast file access:\t%s\n", buf);

                time = localtime(&fileStat.st_mtime);
                memset(buf,0,1024);
                strftime(buf,sizeof(buf), "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", time);
                printf("\tlast file modification:\t%s\n", buf);

                time = localtime(&fileStat.st_ctime);
                memset(buf,0,1024);
                strftime(buf,sizeof(buf), "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", time);
                printf("\tlast status changed:\t%s\n", buf);
    	}

	return(0);
}
