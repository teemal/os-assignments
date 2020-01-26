#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <pthread.h>

#include "socket_hdr.h"

int makeSockandConn(int servPort, char *ipAddr);

void helpFunc(void);
void helpFunc(void) {
	printf("-p | enter a port number\n");
	printf("-u | adds a 1000 sec delay when using put/get \n");
	printf("-h | display help function\n");
}

void dirFunc(cmd_t cmd, int soc);
void dirFunc(cmd_t cmd, int soc){
	ssize_t num_bytes = -1;
	char buf[MAXLINE];

	strcpy(cmd.name, CMD_DIR);
	write(soc, &cmd, sizeof(cmd_t));

	while((num_bytes = read(soc, buf, sizeof(buf))) > 0) {
		write(STDOUT_FILENO, buf, num_bytes);
	}

	close(soc);
	exit(0);
}

void getFunc(cmd_t *cmd, int argc, char *argv[], int servPort, char *);
void getFunc(cmd_t *cmd, int argc, char *argv[], int servPort, char *ipAddr){
	unsigned i;
	int fd;
	char buf[MAXLINE];
	ssize_t num_bytes = -1;
	int soc;
	printf("%d\n", __LINE__);
	for(i = optind; i < argc; i++) {
		soc = makeSockandConn(servPort, ipAddr);
		printf("soc: %d  %d\n", soc, __LINE__);
		fflush(stdout);
		strcpy(cmd->cmd, CMD_GET);
		strcpy(cmd->name, argv[i]);
		//THREAD HERE=============
		num_bytes = write(soc, cmd, sizeof(cmd_t));
		if (num_bytes < 0) {
			perror("cmd_t write failed");
		}
		fd = open(argv[i], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		printf("writing %s\n", cmd->name);
		fflush(stdout);
		if (fd < 0) {
			perror("open of file failed");
			// puke
		}
		printf("%d\n", __LINE__);
		fflush(stdout);
		while((num_bytes = read(soc, buf, sizeof(buf))) > 0) {
			printf("writing %s\n", cmd->name);
			write(fd, buf, num_bytes);
		}
		//======TO HERE======
		if(num_bytes < 0){
			perror("get read error");
		}
		//close(fd);
		//close(soc);
	}
}

void putFunc(cmd_t cmd, int soc, int argc, char *argv[]);
void putFunc(cmd_t cmd, int soc, int argc, char *argv[]){
	unsigned i;
	int fd;
	char buf[MAXLINE];
	ssize_t num_bytes = -1;

	for (i = optind; i < argc; i++) {
		strcpy(cmd.cmd, CMD_PUT);
		strcpy(cmd.name, argv[i]);

		write(soc, &cmd, sizeof(cmd_t));
		fd = open(argv[i], O_RDONLY);
		while((num_bytes = read(fd, buf, sizeof(buf))) > 0) {
			write(soc, buf, num_bytes);
			memset(buf, 0, sizeof(buf));
		}
		if(num_bytes < 0){
			perror("put read error");
		}
	}
	close(fd);
	close(soc);
}

int makeSockandConn(int servPort, char *ipAddr) {
	int soc;
	struct sockaddr_in servaddr;

	soc = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;

	//host-to-network-short for the port number
	servaddr.sin_port = htons(servPort);
	inet_pton(AF_INET, ipAddr, &servaddr.sin_addr.s_addr);

	if(connect(soc, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
		perror("could not connect");
		exit(EXIT_FAILURE);
	}

	return soc;
}


int main(int argc, char *argv[]) {
	int servPort = SERV_PORT;
	int soc;
	int opt;
	//struct sockaddr_in servaddr;

	char ipAddr[50];
	cmd_t cmd;

	while((opt = getopt(argc, argv, CLIENT_OPTIONS)) != -1) {
		switch(opt) {
		case 'p':
			servPort = atoi(optarg);
			break;
		case 'i':
			snprintf(ipAddr, 50, "%s", optarg);
			break;
		case 'c':
			snprintf(cmd.cmd, CMD_LEN, "%s", optarg);
			break;
		case 'v':
			break;
		case 'h':
			helpFunc();
			exit(EXIT_SUCCESS);
			break;
		}//end of switch
	}//end of while

	// soc = socket(AF_INET, SOCK_STREAM, 0);
	// memset(&servaddr, 0, sizeof(servaddr));
	//
	// servaddr.sin_family = AF_INET;
	//
	// //host-to-network-short for the port number
	// servaddr.sin_port = htons(servPort);
	// inet_pton(AF_INET, ipAddr, &servaddr.sin_addr.s_addr);
	//
	// if(connect(soc, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
	// 	perror("could not connect");
	// 	exit(EXIT_FAILURE);
	// }

	soc = makeSockandConn(servPort, ipAddr);
	//==========DIR STUFF==============
	if(strcmp(cmd.cmd, CMD_DIR) == 0) {
		dirFunc(cmd, soc);
	}
	//==========GET STUFF==============
	if (strcmp(cmd.cmd, CMD_GET) == 0) {
		printf("%d\n", __LINE__);
		getFunc(&cmd, argc, argv, servPort, ipAddr);
	}
	//==========PUT STUFF==============
	if (strcmp(cmd.cmd, CMD_PUT) == 0) {
		putFunc(cmd, soc, argc, argv);
	}

	return (EXIT_SUCCESS);
}
