#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "cmd_parse.h"

unsigned short isVerbose = 0;


//int main( int argc, char *argv[] )
int process_user_input_simple(void)
{
    char str[MAX_STR_LEN];
    char *ret_val;
    char *raw_cmd;
    cmd_list_t *cmd_list = NULL;
    int cmd_count = 0;
    char prompt[30];

    // Set up a cool user prompt.
    sprintf(prompt, PROMPT_STR " %s :-) ", getenv("LOGNAME"));
    for ( ; ; ) {
        fputs(prompt, stdout);
        memset(str, 0, MAX_STR_LEN);
        ret_val = fgets(str, MAX_STR_LEN, stdin);

        if (NULL == ret_val) {
            // end of input, a control-D was pressed.
            // Bust out of the input loop and go home.
            break;
        }

        // STOMP on the trailing newline returned from fgets(). I should be
        // more careful about STOMPing on the last character, just in case
        // it is something other than a newline, but I'm being bold and STOMPing.
        str[strlen(str) - 1] = 0;
        if (strlen(str) == 0) {
            // An empty command line.
            // Just jump back to the promt and fgets().
            // Don't start telling me I'm going to get cooties by
            // using continue.
            continue;
        }

        if (strcmp(str, EXIT_CMD) == 0) {
            // Pickup your toys and go home. I just hope there are not
            // any memory leaks.
            break;
        }
        // Basic commands are pipe delimited.
        // This is really for Stage 2.
        raw_cmd = strtok(str, PIPE_DELIM);

        cmd_list = (cmd_list_t *) calloc(1, sizeof(cmd_list_t));

        // This block should probably be put into its own function.
        cmd_count = 0;
        while (raw_cmd != NULL ) {
            cmd_t *cmd = (cmd_t *) calloc(1, sizeof(cmd_t));

            cmd->raw_cmd = strdup(raw_cmd);
            cmd->list_location = cmd_count++;

            if (cmd_list->head == NULL) {
                // An empty list.
                cmd_list->tail = cmd_list->head = cmd;
            }
            else {
                // Make this the last in the list of cmds
                cmd_list->tail->next = cmd;
                cmd_list->tail = cmd;
            }
            cmd_list->count++;

            // Get the next raw command.
            raw_cmd = strtok(NULL, PIPE_DELIM);
        }
        // Now that I have a linked list of the pipe delimited commands,
        // go through each individual command.
        parse_commands(cmd_list);

        // This is a really good place to call a function to exec the
        // the commands just parsed from the user's command line.
        exec_commands(cmd_list);

        // We (that includes you) need to free up all the stuff we just
        // allocated from the heap. That linked list is linked lists looks
        // like it will be nasty to free up, but just follow the memory.
        free_list(cmd_list);
        cmd_list = NULL;
    }

    return(EXIT_SUCCESS);
}

void simple_argv(int argc, char *argv[] )
{
    int opt;

    while ((opt = getopt(argc, argv, "hv")) != -1) {
        switch (opt) {
        case 'h':
            // help
            // Show something helpful
            fprintf(stdout, "You must be out of your Vulcan mind if you think\n"
                    "I'm going to put helpful things in here.\n\n");
            exit(EXIT_SUCCESS);
            break;
        case 'v':
            // verbose option to anything
            // I have this such that I can have -v on the command line multiple
            // time to increase the verbosity of the output.
            isVerbose++;
            if (isVerbose) {
                fprintf(stderr, "verbose: verbose option selected: %d\n", isVerbose);
            }
            break;
        case '?':
            fprintf(stderr, "*** Unknown option used, ignoring. ***\n");
            break;
        default:
            fprintf(stderr, "*** Oops, something strange happened <%c> ... ignoring ...***\n", opt);
            break;
        }
    }
}
static char **history = NULL;
void exec_commands( cmd_list_t *cmds ) 
{
	cmd_t *cmd;
	cmd = cmds->head;
	if (history == NULL) {
		history = calloc(10, sizeof(char *));
	}

    if (1 == cmds->count) {
    	free(history[9]);
    	memmove(&history[1],&history[0], sizeof(char *) * 9);
    	history[0] = cmd->raw_cmd;
        if (!cmd->cmd) {
            // if it is an empty command, bail.
            return;
        }
        if (0 == strcmp(cmd->cmd, CD_CMD)) {
            if (0 == cmd->param_count) {
                // Just a "cd" on the command line without a target directory
                // need to cd to the HOME directory.

                // Is there an environment variable, somewhere, that contains
                // the HOME directory that could be used as an argument to
                // the chdir() fucntion?
                	chdir("$HOME");
            }
            else {
                // try and cd to the target directory. It would be good to check
                // for errors here.
                if (0 == chdir(cmd->param_list->param)) {
                    // a happy chdir!  ;-)
                    chdir("param");
                }
                else {
                    // a sad chdir.  :-(
                    //perror("not a directory");
                  	printf("-benny: %s: Not a directory\n", cmd->param_list->param);
                  	exit(1);
                }

            }
        }
        else if (0 == strcmp(cmd->cmd, PWD_CMD)) {
             char str[MAX_STR_LEN];

            // Fetch the current working directory.
            // aka - get country western dancing
            getcwd(str, MAX_STR_LEN); 
            printf(" " PWD_CMD ": %s\n", str);
        }
        else if (0 == strcmp(cmd->cmd, ECHO_CMD)) {
            // insert code here
            // insert code here
            // Is that an echo?
        	param_t *tmp = cmd->param_list;

        	while (tmp) {
        		printf("%s ", tmp->param);
        		tmp = tmp->next;
        	}
        	printf("\n");
        }
        else if (0 == strcmp(cmd->cmd, HISTORY_CMD)) {
            // look back in time
        	int i;
        	for(i = 9; i >= 0; i--){
        		if(history[i] != NULL){
        			printf("%s\n", history[i]);
        		}
        	}
        }
        else {
            // a single command to create and exec
            // if you really do things correctly, you don't need a special
            // for a single command, as distinguished from multiple commands.
            //printf("TEST\n");
        	pid_t pid;
        	int *status = 0;
        	//int fileDes[2] = {-1, -1};
        	int iFd = STDIN_FILENO;
        	int oFd = STDOUT_FILENO;

        	if ((pid = fork()) == -1) {
   				perror("BAD FORK ---E\n");
   				exit(1);
   			}
			if (pid == 0) { //child A process
    			if (cmd->input_src == REDIRECT_FILE){
    				/* code */
    				iFd = open(cmd->input_file_name, O_RDONLY);
    				dup2(iFd, STDIN_FILENO);
    				close(iFd);    			
    			}
    			if (cmd->input_src == REDIRECT_FILE){
    				oFd = open(cmd->output_file_name, O_WRONLY | O_CREAT | O_TRUNC);
    				dup2(oFd, STDOUT_FILENO);
    				close(oFd);
    			}
    			{
    			param_t *head = cmd->param_list;
                int  size = cmd->param_count + 2;
                char *listOfCmds[size];
                int location = 1;
                listOfCmds[0] = cmd->cmd;
                while (head != NULL) {
                        listOfCmds[location] = head->param;
                        location++;
                        head = head->next;
                }
                listOfCmds[size-1] = (char *)NULL;
                execvp(listOfCmds[0], listOfCmds);
                perror("exec failed");
            	}
    		}
    		else{ //parent A process
    			//
    			pid = wait(status);
    		}
    	}
    }
    else {
        // Other things???
        // More than one command on the command line. Who'da thunk it!
        // This really falls into Stage 2.
        printf("test\n");
    }
}
void free_list(cmd_list_t *cmd_list)
{
    // Proof left to the student.
    // You thought I was going to do this for you! HA! You get
    // the enjoyment of doing it for yourself.
    free_cmd(cmd_list->head);
    free(cmd_list);
}

void print_list(cmd_list_t *cmd_list)
{
    cmd_t *cmd = cmd_list->head;

    while (NULL != cmd) {
        print_cmd(cmd);
        cmd = cmd->next;
    }
}

void free_cmd (cmd_t *cmd)
{
   // Proof left to the student.
   // Yep, on yer own.
   param_t *param = cmd->param_list;
    while(cmd != NULL){
        cmd_t *temp_cmd = cmd;
        free(cmd->raw_cmd);
        free(cmd->cmd);
        free(cmd->input_file_name);
        free(cmd->output_file_name);
        while(param != NULL){
            param_t *temp_param = param;
            free(param->param);
            param = param->next;        
            free(temp_param);
        }
        cmd = cmd->next;
        free(temp_cmd);
    }
}

// Oooooo, this is nice. Show the fully parsed command line in a nice
// asy to read and digest format.
void print_cmd(cmd_t *cmd)
{
    param_t *param = NULL;
    int pcount = 1;

    fprintf(stderr,"raw text: +%s+\n", cmd->raw_cmd);
    fprintf(stderr,"\tbase command: +%s+\n", cmd->cmd);
    fprintf(stderr,"\tparam count: %d\n", cmd->param_count);
    param = cmd->param_list;

    while (NULL != param) {
        fprintf(stderr,"\t\tparam %d: %s\n", pcount, param->param);
        param = param->next;
        pcount++;
    }

    fprintf(stderr,"\tinput source: %s\n"
            , (cmd->input_src == REDIRECT_FILE ? "redirect file" :
               (cmd->input_src == REDIRECT_PIPE ? "redirect pipe" : "redirect none")));
    fprintf(stderr,"\toutput dest:  %s\n"
            , (cmd->output_dest == REDIRECT_FILE ? "redirect file" :
               (cmd->output_dest == REDIRECT_PIPE ? "redirect pipe" : "redirect none")));
    fprintf(stderr,"\tinput file name:  %s\n"
            , (NULL == cmd->input_file_name ? "<na>" : cmd->input_file_name));
    fprintf(stderr,"\toutput file name: %s\n"
            , (NULL == cmd->output_file_name ? "<na>" : cmd->output_file_name));
    fprintf(stderr,"\tlocation in list of commands: %d\n", cmd->list_location);
    fprintf(stderr,"\n");
}

// Remember how I told you that use of alloca() is
// dangerous? You can trust me. I'm a professional.
// And, if you mention this in class, I'll deny it
// ever happened. What happens in stralloca stays in
// stralloca.
#define stralloca(_R,_S) {(_R) = alloca(strlen(_S) + 1); strcpy(_R,_S);}

void parse_commands(cmd_list_t *cmd_list)
{
    cmd_t *cmd = cmd_list->head;
    char *arg;
    char *raw;

    while (cmd) {
        // Because I'm going to be calling strtok() on the string, which does
        // alter the string, I want to make a copy of it. That's why I strdup()
        // it.
        // Given that command lines should not be tooooo long, this might
        // be a reasonable place to try out alloca(), to replace the strdup()
        // used below. It would reduce heap fragmentation.
        //raw = strdup(cmd->raw_cmd);

        // Following my comments and trying out alloca() in here. I feel the rush
        // of excitement from the pending doom of alloca(), from a macro even.
        // It's like double exciting.
        stralloca(raw, cmd->raw_cmd);

        arg = strtok(raw, SPACE_DELIM);
        if (NULL == arg) {
            // The way I've done this is like ya'know way UGLY.
            // Please, look away.
            // If the first command from the command line is empty,
            // ignore it and move to the next command.
            // No need free with alloca memory.
            //free(raw);
            cmd = cmd->next;
            // I guess I could put everything below in an else block.
            continue;
        }
        // I put something in here to strip out the single quotes if
        // they are the first/last characters in arg.
        if (arg[0] == '\'') {
            arg++;
        }
        if (arg[strlen(arg) - 1] == '\'') {
            arg[strlen(arg) - 1] = '\0';
        }
        cmd->cmd = strdup(arg);
        // Initialize these to the default values.
        cmd->input_src = REDIRECT_NONE;
        cmd->output_dest = REDIRECT_NONE;

        while ((arg = strtok(NULL, SPACE_DELIM)) != NULL) {
            if (strcmp(arg, REDIR_IN) == 0) {
                // redirect stdin

                //
                // If the input_src is something other than REDIRECT_NONE, then
                // this is an improper command.
                //

                // If this is anything other than the FIRST cmd in the list,
                // then this is an error.

                cmd->input_file_name = strdup(strtok(NULL, SPACE_DELIM));
                cmd->input_src = REDIRECT_FILE;
            }
            else if (strcmp(arg, REDIR_OUT) == 0) {
                // redirect stdout
                       
                //
                // If the output_dest is something other than REDIRECT_NONE, then
                // this is an improper command.
                //

                // If this is anything other than the LAST cmd in the list,
                // then this is an error.

                cmd->output_file_name = strdup(strtok(NULL, SPACE_DELIM));
                cmd->output_dest = REDIRECT_FILE;
            }
            else {
                // add next param
                param_t *param = (param_t *) calloc(1, sizeof(param_t));
                param_t *cparam = cmd->param_list;

                cmd->param_count++;
                // Put something in here to strip out the single quotes if
                // they are the first/last characters in arg.
                if (arg[0] == '\'') {
                    arg++;
                }
                if (arg[strlen(arg) - 1] == '\'') {
                    arg[strlen(arg) - 1] = '\0';
                }
                param->param = strdup(arg);
                if (NULL == cparam) {
                    cmd->param_list = param;
                }
                else {
                    // I should put a tail pointer on this.
                    while (cparam->next != NULL) {
                        cparam = cparam->next;
                    }
                    cparam->next = param;
                }
            }
        }
        // This could overwite some bogus file redirection.
        if (cmd->list_location > 0) {
            cmd->input_src = REDIRECT_PIPE;
        }
        if (cmd->list_location < (cmd_list->count - 1)) {
            cmd->output_dest = REDIRECT_PIPE;
        }

        // No need free with alloca memory.
        //free(raw);
        cmd = cmd->next;
    }

    if (isVerbose > 0) {
        print_list(cmd_list);
    }
}

int main(int argc, char *argv[]) {
    int ret = 0;
    int i;

    simple_argv(argc, argv);
    ret = process_user_input_simple();
    for(i = 0; i < 10; i++){
        free(history[i]);
    }
    free(history);

    return(ret);
}
