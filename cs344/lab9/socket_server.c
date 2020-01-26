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

#define LISTENQ 1

void helpFunc(void);
void helpFunc(void) {
	printf("-p | enter a port number\n");
	printf("-u | adds a 1000 sec delay when using put/get \n");
	printf("-h | display help function\n");
}

void dirFunc(char buf[], int sockfd);
void dirFunc(char buf[], int sockfd){
	FILE *dir = popen(CMD_DIR_POPEN, "r");
	if (dir != NULL) {
		while (fgets(buf, MAXLINE, dir) != NULL) {
			write(sockfd, buf, strlen(buf));
		}
		pclose(dir);
		close(sockfd);
	}
	else {
		perror("popen for dir failed");
	}
}

void getFunc(cmd_t *cmd);
void getFunc(cmd_t *cmd){
	int fd;
	ssize_t num_bytes = -1;
	char buf[MAXLINE] = {'\0'};

	printf("soc: %d  %d\n", cmd->sock, __LINE__);
	fd = open(cmd->name, O_RDONLY);
	if (fd < 0) {
		perror("open of file failed");
	}
	while((num_bytes = read(fd, buf, sizeof(buf))) > 0) {
		num_bytes = write(cmd->sock, buf, num_bytes);
		if(num_bytes < 0){
			perror("error on get write");
		}
		memset(buf, 0, sizeof(buf));
	}
	close(fd);
	close(cmd->sock);
	free(cmd);
}

void putFunc(cmd_t *cmd);
void putFunc(cmd_t *cmd){
	int fd;
	ssize_t num_bytes = -1;
	char buf[MAXLINE] = {'\0'};

	fd = open(cmd->name, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	printf("%d\n", __LINE__);
	while((num_bytes = read(cmd->sock, buf, sizeof(buf))) > 0) {
		printf("%d\n", __LINE__);
		if(num_bytes < 0){
			perror("error on put read");
		}
		printf("%d\n", __LINE__);
		write(fd, buf, num_bytes);
		memset(buf, 0, sizeof(buf));
	}

	close(fd);
}

int main(int argc, char *argv[]) {
	int listenfd;
	int delayFiles = 0;
	int n;
	int opt;
	short servPort = SERV_PORT;
	socklen_t clilen;
	int sockfd;
	//cmd_t cmd;
	struct sockaddr_in cliaddr;
	struct sockaddr_in servaddr;
	char buf[MAXLINE];

	while((opt = getopt(argc, argv, SERVER_OPTIONS)) != -1) {
		switch(opt) {
			case 'p':
				servPort = atoi(optarg);
				break;
			case 'u':
				delayFiles++; //do usleep(1000)
				break;
			case 'h':
				helpFunc();
				exit(EXIT_SUCCESS);
				break;
		}//end of switch
	}//end of while

	//create a socket stream IPV4
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));

	//an IPv4 address
	servaddr.sin_family = AF_INET;
	//host-to-network-long . listen on any interface/ip of the system
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//host-to-network-short, the default port from above
	servaddr.sin_port = htons(servPort);

	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
		perror("Something is not working");
		exit(-1);
	}

	listen(listenfd, LISTENQ);
	printf("server listening on port: %d\n", servPort);

	{
		char hostname[1024];
		struct addrinfo hints;
		struct addrinfo *addr_list;
		struct addrinfo *p;
		int status;
		char ipstr[INET6_ADDRSTRLEN];
		gethostname(hostname, 1024);

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		if((status = getaddrinfo(hostname, NULL, &hints, &addr_list) != 0)) {
			fprintf(stderr, "getaddrinfo: %s  %s\n", gai_strerror(status), hostname);
			return(2);
		}

		for(p = addr_list; p != NULL; p = p->ai_next) {
			void *addr;

			if(p->ai_family == AF_INET) {
				struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
				addr = &(ipv4->sin_addr);
			}

			inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
			printf("Hostname: %s  IP address: %s\n", hostname, ipstr);
			freeaddrinfo(addr_list);
		}
	}
	//client length
	clilen = sizeof(cliaddr);

	//accept conntecions on the listenfd
	//sockfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
	while (1) {
		cmd_t *tcmd = NULL;
		//accept conntections
		printf("%d\n", __LINE__);
		sockfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
		if(sockfd == -1) {
			perror("something went wrong");
			exit(EXIT_FAILURE);
		}
		printf("soc: %d   %d\n", sockfd, __LINE__);
		tcmd = calloc(1, sizeof(cmd_t));
		memset(buf, 0, sizeof(buf));
		if((n = read(sockfd, tcmd, sizeof(cmd_t))) == 0) {
			printf("EOF found on client connection socket, exiting\n");
			close(sockfd);
			//break;
		} else {
			printf(">%s<  %d\n", tcmd->cmd, __LINE__);
			fflush(stdout);
			//==========DIR STUFF==============
			if (strcmp(tcmd->cmd, CMD_DIR) == 0) {
				dirFunc(buf, sockfd);
			}
			//==========GET STUFF==============
			if (strcmp(tcmd->cmd, CMD_GET) == 0) {
				printf(">%s<  %d\n", tcmd->cmd, __LINE__);
				fflush(stdout);
				tcmd->sock = sockfd;
				getFunc(tcmd);
				//pth.. create(&tid, NULL, getFunc, (void *) tcmd);
			}
			//==========PUT STUFF==============
			if(strcmp(tcmd->cmd, CMD_PUT) == 0) {
				tcmd->sock = sockfd;
				//putFunc(tcmd);
			}
			//-----------------------------------//
			printf(">%s<  %d\n", tcmd->cmd, __LINE__);
			fflush(stdout);
		}
		printf(">%s<  %d\n", tcmd->cmd, __LINE__);
		fflush(stdout);

	}
	//printf(">%s<  %d\n", tcmd->cmd, __LINE__);
	printf("Closing listen socket\n");
	//close(sockfd);
	close(listenfd);
	return (EXIT_SUCCESS);
}
