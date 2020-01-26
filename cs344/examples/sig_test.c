#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

#define BUFF 1024

void handler_term(int);
void handler_int(int);
void handler_alrm(int);

void handler_term(int sig) {
         if(sig == SIGTERM) {
                printf("\nSignal Cought a %d: %s (in function %s)\n", sig, sys_siglist[sig], __func__);
                exit(0);
        }
}

void handler_int(int sig) {
	int i;
	char str_yes[BUFF] = {"yes"};
        char answer[BUFF];
        if(sig == SIGINT) {
                printf("would you like to exit?: ");
                scanf("%s", answer);
                //printf("%s", answer);
                for(i = 0; answer[i]; i++){
			answer[i] = tolower(answer[i]);
		}
		if(strcmp(str_yes, answer) == 0){
			printf("\nkilled!");
			exit(0);
		}
        }
}

void handler_alrm(int sig) {
	time_t curr_time;
	time(&curr_time);
	printf("\nCurrent Time: %s", ctime(&curr_time));
}

void handler_usr(int sig){
	if(sig == SIGUSR1){
		printf("\nCaught: SIGUSR1");
	}
	if(SIGUSR2){
		printf("\nCaught: SIGUSR2");
	}
}

void handler_kill(int sig){
	printf("Called: SIGKILL");
}

int main(int argc, char *argv[], char *envp[]) {
       	alarm(3); //calls SIGALRM  after 3 sec
	printf("Here is the ID: %d\n", getpid());

        void *sig_ret;
        sig_ret = signal(SIGTERM, handler_term);
         if (sig_ret == SIG_ERR) {
                perror("An error trying to set signal on SIGINT");
        }
	if(signal(SIGKILL, handler_kill) == SIG_ERR){
		printf("\n Can't catch SIGKILL");
	}
        sig_ret = signal(SIGINT, handler_int);
	sig_ret = signal(SIGALRM, handler_alrm);
	sig_ret = signal(SIGUSR1, handler_usr);
	sig_ret = signal(SIGUSR2, handler_usr);
        if (sig_ret == SIG_ERR) {
                perror("An error trying to set signal on SIGINT");
        }

    while(1) {
                sleep(1);
            }
        return(EXIT_SUCCESS);
}
