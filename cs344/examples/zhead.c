#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <>string.h>
#include <stdio.h>
#include <libgen.h>

int main(int argc, char* argv[]){
	char* lines = NULL;
	char* file = NULL;
	{
		char c= 0;
		while((c = getopt(argc, argv, "hn:")) != -1){
			switch (c){
			case 'n':
				lines = strdup(optarg);
				break;
			case 'h':
				printf("are you crazy!");
				exit(EXIT_SUCCESS);
			default:
				exit(EXIT_SUCCESS);
			}
		}
	}
	if(optind < argc){
		file_name = strdup(argv[optind]);
	}
	else{
		exit(EXIT_SUCCESS);
	}
	{
		int pipes[2] = {-1,-1};
		pid_t = fork();
		switch(pid){
		case -1:
			perror("POOPY");
			exit(EXIT_SUCCESS);
			break;
		}
		case 0:
			{
				char **rhp_argv = NULL;
				char* rhp = NULL;

				if (dup2(pipes[STDIN_FILENO], STDIN_FILNO) < 0){
					perror("POOPIE");
					_exit(EXIT_FAILURE);
				}
				close(pipes[STDIN_FILEON]);
				close(pipes[STDOUT_FILENO]);

				rhp = strdup("head");
				if(NULL != lines){
					rhp_argv = calloc(4, sizeof(char *));
					rhp_argv[0] = rhp;
					rhp_argv[1] = strdup("-n");
					rhp_argv[2] = lines;
				}
				else{
					rhp_argv = calloc(2, sizeof(char *));
					rhp_argv[0] = rhp;
				}
				execlp(rhp_argv[0], rhp_argv);
				perror("FAIL");
				_exit(EXIT_FALURE);
			}
			break;
		case default:
			{
				char lhp_argv[] = {
					"gunzip"
					, "-c"
					, file_name
					, (char *)  NULL;
				};
			}
		}
	}
	return(EXIT_FAILURE);
}
