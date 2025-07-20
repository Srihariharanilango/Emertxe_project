#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 3

char *external_commands[200];
int status;
int pid = -1;

struct job {
    int data;
    char *str;
    struct job* link;
};

struct job *head = NULL;
char input_string[100];
char prompt[100] = "Minishell$:";


void add_job(int pid, const char *cmd) {
    struct job *newnode = (struct job*) malloc(sizeof(struct job));
    newnode->data = pid;
    newnode->str = strdup(cmd);
    newnode->link = head;
    head = newnode;
}

void remove_job(int pid) {
    struct job *curr = head, *prev = NULL;
    while (curr != NULL) {
        if (curr->data == pid) {
            if (prev == NULL)
                head = curr->link;
            else
                prev->link = curr->link;
            free(curr->str);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->link;
    }
}

void print_jobs() {
    struct job* temp = head;
    while (temp != NULL) {
        printf("[%d] Stopped\t%s\n", temp->data, temp->str);
        temp = temp->link;
    }
}

void bring_fg() {
    if (head == NULL) {
        printf("fg: no current job\n");
        return;
    }
    int fg_pid = head->data;
    char *fg_cmd = strdup(head->str);
    remove_job(fg_pid);
    printf("%s\n", fg_cmd);
    kill(fg_pid, SIGCONT);
    pid = fg_pid;
    waitpid(fg_pid, &status, WUNTRACED);
    pid = -1;
    free(fg_cmd);
}

void run_bg() {
    if (head == NULL) {
        printf("bg: no current job\n");
        return;
    }
    printf("[%d] %s &\n", head->data, head->str);
    kill(head->data, SIGCONT);
}


void signal_handler(int sig_num) {
    if (sig_num == SIGINT || sig_num == SIGTSTP) {
        if (pid == -1) {
            printf("\n%s", prompt);
            fflush(stdout);
        } else if (sig_num == SIGTSTP) {
            add_job(pid, input_string);
            kill(pid, SIGTSTP);
        }
    }
}


char *get_command(char *input_string) {
    char *arr = malloc(strlen(input_string) + 1);
    int i = 0;
    while (input_string[i] != '\0' && input_string[i] != ' ') {
        arr[i] = input_string[i];
        i++;
    }
    arr[i] = '\0';
    return arr;
}

int check_command_type(char *command) {
    char *builtins[] = {"echo", "cd", "pwd", "exit", "jobs", "fg", "bg", NULL};
    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(builtins[i], command) == 0)
            return BUILTIN;
    }
    for (int i = 0; external_commands[i] != NULL; i++) {
        if (strcmp(external_commands[i], command) == 0)
            return EXTERNAL;
    }
    return NO_COMMAND;
}


void scan_input(char *prompt, char *input_string) {
    while (1) {
        printf("%s", prompt);
        scanf(" %[^\n]", input_string);
        __fpurge(stdin);

        if (strncmp(input_string, "PS1=", 4) == 0 && input_string[4] != ' ') {
            strcpy(prompt, input_string + 4);
            continue;
        }

        char *cmd = get_command(input_string);
        int cmd_type = check_command_type(cmd);

        if (cmd_type == BUILTIN) {
            if (strcmp(cmd, "jobs") == 0)
                print_jobs();
            else if (strcmp(cmd, "fg") == 0)
                bring_fg();
            else if (strcmp(cmd, "bg") == 0)
                run_bg();
            else if (strcmp(cmd, "exit") == 0)
                exit(0);
            else
                execute_internal_commands(input_string);
        }

        else if (cmd_type == EXTERNAL) {
            pid = fork();
            if (pid == 0) {
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                execute_external_commands(input_string);
                exit(0);
            } else if (pid > 0) {
                waitpid(pid, &status, WUNTRACED);
                pid = -1;
            }
        }

        else {
            printf("Invalid command: %s\n", cmd);
        }

        free(cmd);
    }
}

int main() {
    system("clear");
    extract_external_commands(external_commands);
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    scan_input(prompt, input_string);
    return 0;
}

