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

int main(int argc, char *argv[]) {
  int delay;
  int port;
  int opt;

    while ((opt = getopt(argc, argv, "pu:")) != -1) {
        switch (opt) {
        case 'p':
            port = optarg;
            printf("attempting to connect to port# %d\n", port);
            break;
        case 'u':
            delay = optarg;
            printf("adding %d microsecond delay\n", delay);
            break;
        case '?':
            break;
        }
    }
  return 0;
}
