#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

#define MAX_BUFF 1024

void closePipes(int filedes[], int filedes2[], int filedes3[]);
void closePipes(int filedes[], int filedes2[], int filedes3[]){
	close(filedes[STDIN_FILENO]);
	close(filedes[STDOUT_FILENO]);

	close(filedes2[STDIN_FILENO]);
        close(filedes2[STDOUT_FILENO]);

	close(filedes3[STDIN_FILENO]);
        close(filedes3[STDOUT_FILENO]);
}

int main(int argc, char *argv[]) {
	int opt;
	char* numInput = NULL;
	int filedes1[2] = {-1, -1};
	int filedes2[2] = {-1, -1};
	int filedes3[2] = {-1, -1};
	pid_t pid_a, pid_b, pid_c;
	pipe(filedes1);
	pipe(filedes2);
	pipe(filedes3);

	while((opt = getopt(argc, argv, "n:")) != -1){
		switch(opt){
			case 'n':
				numInput = optarg;
                		break;
			case '?':
				perror("Unknown Command\n");
			}
	}
	//=====================================================================
   	//========FORk 1================================
	if ((pid_a = fork()) == -1) {
   		perror("BAD FORK ---E\n");
   		exit(1);
   	}
	if (pid_a == 0) { //child A process
    		dup2(filedes1[STDOUT_FILENO], STDOUT_FILENO);
    		closePipes(filedes1, filedes2, filedes3);
    		execlp("ls", "ls", NULL);
		perror("ls exec error :(");
        }
	//========FORk 2================================
	if ((pid_b = fork()) == -1) {
                perror("BAD FORK ---E\n");
                exit(1);
        }
        if (pid_b == 0) { //child B process
		dup2(filedes1[STDIN_FILENO], STDIN_FILENO);
                dup2(filedes2[STDOUT_FILENO], STDOUT_FILENO);
                closePipes(filedes1, filedes2, filedes3);
                execlp("xargs", "xargs", "du", "-s", NULL);
		perror("xargs exec error :(");
        }
	//========FORk 3================================
	if ((pid_c = fork()) == -1) {
                perror("BAD FORK ---E\n");
                exit(1);
        }
        if (pid_c == 0) { //child C process
		dup2(filedes2[STDIN_FILENO], STDIN_FILENO);
                dup2(filedes3[STDOUT_FILENO], STDOUT_FILENO);
                closePipes(filedes1, filedes2, filedes3);
                execlp("sort", "sort", "-nr", NULL);
		perror("sort exec error");
        }
        else{		//parent for head
		if(NULL != numInput){
			dup2(filedes3[STDIN_FILENO], STDIN_FILENO);
			closePipes(filedes1, filedes2, filedes3);
			execlp("head", "head", "-n", numInput, NULL);
			perror("head exec error :(");
		}
		else{
			dup2(filedes3[STDIN_FILENO], STDIN_FILENO);
                        closePipes(filedes1, filedes2, filedes3);
                        execlp("head", "head", NULL);
			perror("head exec error :(");
		}
        }
	return(EXIT_FAILURE);
}

