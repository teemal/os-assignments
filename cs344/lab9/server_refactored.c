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
#include <netdb.h>

#define LISTENQ 1
#define MAXLINE 4096

#define QUOTE(_NAME_) #_NAME_
#define STR(_NAME_) QUOTE(_NAME_)

#define SERV_PORT 10001
#define SERV_PORT_STR STR(SERV_PORT)

void printIP(char hostname[], struct addrinfo hints, struct addrinfo *addr_list, struct addrinfo *p, int status, char ipstr[]);
void prepSockets(long port, struct sockaddr_in servaddr, int listenfd, socklen_t clilen, int sockfd, struct sockaddr_in cliaddr);
void sockInOut(int n, int sockfd, char buf[]);

int main(int argc, char *argv[]) {
  int delay;
  long port;
  int opt;
//========ip address print stuff=======
  char hostname[1024];
  struct addrinfo hints;
  struct addrinfo *addr_list;
  struct addrinfo *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];
//============Socket stuff==============
  int listenfd;
  int sockfd;
  int n;
  char buf[MAXLINE];
  socklen_t clilen;
  struct sockaddr_in cliaddr;
  struct sockaddr_in servaddr;

  while ((opt = getopt(argc, argv, "p:u:")) != -1) {
      switch (opt) {
      case 'p':
          port = atol(optarg);
          printf("attempting to connect to port #%ld\n", port);
          break;
      case 'u':
          delay = atoi(optarg);
          printf("adding %d microsecond delay\n", delay);
          break;
      case '?':
          break;
      }
  }

  prepSockets(port, servaddr, listenfd, clilen, sockfd, cliaddr);
  sockInOut(n, sockfd, buf);

  close(listenfd);
  return(EXIT_SUCCESS);
}


void printIP(char hostname[], struct addrinfo hints, struct addrinfo *addr_list, struct addrinfo *p, int status, char ipstr[]){
  gethostname(hostname, 1024);
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      if((status = getaddrinfo(hostname, NULL, &hints, &addr_list) != 0)) {
          fprintf(stderr, "getaddrinfo: %s  %s\n", gai_strerror(status), hostname);
          return(2);
      }
      printf("IP addresses for %s: \n", hostname);
      for(p = addr_list; p != NULL; p = p->ai_next) {
          void *addr;
          char *ipver;
          if(p->ai_family == AF_INET) {
              struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
              addr = &(ipv4->sin_addr);
              ipver = "IPv4 address";
          }
          inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
          printf("%s: %s\n", ipver, ipstr);
          freeaddrinfo(addr_list);
      }
}

void prepSockets(long port, struct sockaddr_in servaddr, int listenfd, socklen_t clilen, int sockfd, struct sockaddr_in cliaddr){
  // Create a socket from the AF_INET family, that is a stream
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  // Performing a memset() is quite important when doing
  //   socket communication.
  memset(&servaddr, 0, sizeof(servaddr));

  // An IPv4 address
  servaddr.sin_family = AF_INET;
  // Host-TO-Network-Long. Listen on any interface/IP of the system.
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  // Host-TO-Network-Short, the default port from above.
  servaddr.sin_port = htonl(port);

  if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
    perror("Something is not working");
    exit(-1);
  }
  // Set up a passive socket, with a backlog of 1. Reject any connection
  // attempts after 1 is already waiting. The return value from this should
  // be checked for error.
  listen(listenfd, LISTENQ);

  printf("server listening on %ld\n", port);

  // client length
  clilen = sizeof(cliaddr);
  // Accept connections on the listenfd.
  sockfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
}

void sockInOut(int n, int sockfd, char buf[]){
  for ( ; ; ) {
    // You REALLY want to memset to all zeroes before you get bytes from
    // the socket.
    memset(buf, 0, sizeof(buf));
    if ((n = read(sockfd, buf, sizeof(buf))) == 0) {
      printf("EOF found on client connection socket, exiting\n");
      // nothing was read, EOF
      close(sockfd);
      // break out of the loop all is done
      break;
    }
    else {
      fprintf(stdout, "message from client: <%s>\n", buf);
      // echo the line back to the client
      write(sockfd, buf, n);
    }
  }
}
