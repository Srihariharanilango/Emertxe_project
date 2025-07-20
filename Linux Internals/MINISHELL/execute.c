#include "main.h"

void execute_external_commands(char *input_string)
{
    char *argv[20];
    int i = 0;
    char *token = strtok(input_string, " ");
    while(token)
    {
        argv[i] = malloc(strlen(token)+1);
        strcpy(argv[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;

    int pipe_count = 0;
    // Count number of pipes
    for(int i = 0; argv[i] != NULL; i++) {
        if(strcmp(argv[i], "|") == 0) {
            pipe_count++;
        }
    }

    if(pipe_count == 0) {
        // no pipe
        execvp(argv[0], argv);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else {
        int pipefd[2 * pipe_count]; // pipefd[0], pipefd[1], pipefd[2], pipefd[3]...

        for(int i = 0; i < pipe_count; i++) {
            if(pipe(pipefd + i*2) < 0) {
                perror("pipe error");
                exit(EXIT_FAILURE);
            }
        }

        int cmd = 0; // command number
        int j = 0;   // index to traverse argv

        while(cmd <= pipe_count) {
            char *cmd_argv[10]; // to store each command's arguments
            int k = 0;

            // split command arguments until pipe or NULL
            while(argv[j] != NULL && strcmp(argv[j], "|") != 0) {
                cmd_argv[k++] = argv[j++];
            }
            cmd_argv[k] = NULL; // NULL terminate the command args

            pid_t pid = fork();
            if(pid < 0) {
                perror("fork error");
                exit(EXIT_FAILURE);
            }

            if(pid == 0) { // Child Process
                // If not first command, read from previous pipe
                if(cmd != 0) {
                    dup2(pipefd[(cmd - 1)*2], 0);
                }

                // If not last command, write to next pipe
                if(cmd != pipe_count) {
                    dup2(pipefd[cmd*2 + 1], 1);
                }

                // Close all pipe fds in child
                for(int l = 0; l < 2 * pipe_count; l++) {
                    close(pipefd[l]);
                }

                execvp(cmd_argv[0], cmd_argv);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }

            cmd++;
            j++; // skip '|'
        }

        // Close all pipe fds in parent
        for(int i = 0; i < 2 * pipe_count; i++) {
            close(pipefd[i]);
        }

        // Wait for all children
        for(int i = 0; i <= pipe_count; i++) {
            wait(NULL);
        }
    }
}

//execution of internal commands
void execute_internal_commands(char *input_string){
  if(strcmp(input_string, "exit")==0){
    printf("Exiting from the minishell..\n");
    exit(0);
  }
  
  else if(strncmp(input_string, "cd ",3)==0)
  {
    int ret = chdir(input_string+3);
    if(ret == -1){
      perror("chdir");
    }
  }
  else if(strcmp(input_string,"pwd") == 0){
    printf("%s\n",getcwd(NULL,0)); //to not store any value just print pwd
  }
  
  else if(strcmp(input_string, "echo$$")==0){
    printf("%d\n",getpid());
  }
  
  else if(strcmp(input_string, "echo$?")==0){
    printf("%d\n",WEXITSTATUS(status));
  }
  else if(strcmp(input_string, "echo $SHELL")==0){
    printf("%s\n",getenv("SHELL"));
  }

}
