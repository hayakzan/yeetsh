/*
  
   yeetsh, a shell with a horrible name

*/

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/* Global variables */
/* The array for holding shell paths */
char shell_paths[MAX_ENTRIES_IN_SHELLPATH][MAX_CHARS_PER_CMDLINE];
static char prompt[] = "yeetsh> "; /* Command line prompt */
static char *default_shell_path[2] = {"/bin", NULL};
/* End Global Variables */

/* Convenience struct for describing a command.*/
struct Command {
  char** args;      /* Argument array for the command */
  char* outputFile; /* Redirect target for file (NULL means no redirect) */
};

char** tokenize_command_line(char* cmdline);
struct Command parse_command(char** tokens);
void eval(char** cmd_tokens);
int try_exec_builtin(struct Command** cmd);
void exec_external_cmd(struct Command** cmd);

int main(int argc, char** argv) {
  
  while (1) {
    printf("%s", prompt);

    /* Read */
    size_t max_chars = MAX_CHARS_PER_CMDLINE;
    char* line = malloc(sizeof(max_chars));
    ssize_t line_len = getline(&line, &max_chars, stdin);
    if(line_len == -1) {
      exit(0);
    }
     
    /* Evaluate */
    else if(line_len > 1) {
      line[line_len - 1] = 0; 
      char** cmd_tokens = tokenize_command_line(line);
      eval(cmd_tokens);
      free(cmd_tokens);
    }
    /* Print */
    free(line);
  }
  return 0;
}


/* Turn a command line into tokens with strtok */
#define TOK_DELIM " " 
int globCount = 0;

char** tokenize_command_line(char* cmdline) {
  int bufsize = MAX_WORDS_PER_CMDLINE;
  char** tokens = malloc(bufsize * sizeof(char*));
  char* token = strtok(cmdline, TOK_DELIM);
  int count = 0;

  while (token != NULL) {
    tokens[count] = token;
    count++;
    token = strtok(NULL, TOK_DELIM);
  }
  //  printf("count: %d\n", count);
  return tokens;
}

/* Turn tokens into a command.*/

struct Command parse_command(char** tokens) {
  char* output = NULL;
  output = tokens[0];
  
  //Aggregate initialization 
   struct Command rtn = {.args = tokens,
  			.outputFile = output};
  return rtn;
}


/* Evaluate a single command */
void eval(char** cmd_tokens) {
  int pos = 0;
  
  struct Command* commands = (struct Command*)malloc(sizeof(struct Command));

  while(pos < 2) { //TODO: count the args
    commands[pos] = parse_command(tokenize_command_line(cmd_tokens[pos]));
    pos++;
  }
  
  try_exec_builtin(&commands);
  exec_external_cmd(&commands);
  free(commands);
}

/* Execute built-in commands */

char* builtin_str[] = { "exit", "cd", "path?", "help" };

int yeetsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int try_exec_builtin(struct Command** cmd) {
  struct Command* curr_cmd = (struct Command*)malloc(sizeof(struct Command));

  char* exitMsg = "exit";
  char* cdMsg = "cd";
  char* pathMsg = "path?";
  char* helpMsg = "help";
  char* donutMsg = "donut";
  char s[100];
  int ret;
  
  // curr_cmd = (*cmd);
  
  for (int i = 0; i < 2; i++) { 
    curr_cmd = (*cmd);   
  }
  
  if (curr_cmd[0].args[0] == NULL) {
    printf("input is NULL\n");
    return 1;
  }
  else if (strcmp(curr_cmd[0].args[0], exitMsg) == 0) {
    printf("Have a good one.\n");
    kill(0, SIGINT);
  }
  else if (strcmp(curr_cmd[0].args[0], pathMsg) == 0) {
    printf("current dir: %s\n", getcwd(s, 100));
  }

  else if (strcmp(curr_cmd[0].args[0], helpMsg) == 0) {
    int i;
    printf("yeetsh: a shell with a horrible name\n");
    printf("type 'donut' for a free doughnut (code by Andy Sloane, see the details in donut.c)\n");
    printf("the following builtin commands are available:\n");
    for (i=0; i < yeetsh_num_builtins(); i++) {
      printf("   %s\n", builtin_str[i]);
    }
    return 1;
  }
  else if (strcmp(curr_cmd[0].args[0], cdMsg) == 0) { //TODO: error if there are > 2 args after cd command
    ret = chdir(curr_cmd[1].args[0]);
    if (ret != 0) {
      perror("?! yeetsh!");
    }
  }
  else if (strcmp(curr_cmd[0].args[0], cdMsg) == 0) {
    char* path = getcwd(s, 100);
    execv(path, curr_cmd[0].args[0]);
  }
return 0;
}


/* Execute an external command */

void exec_external_cmd(struct Command** cmd) {
  struct Command* curr_cmd = (struct Command*)malloc(sizeof(struct Command));
  //  struct Command curr_cmd;
  int cmd_pr = fork();
  curr_cmd = (*cmd);
  
  if (cmd_pr < 0) {
    //fork failed
    //    fprintf(stderr, "fork failed\n");
    exit(1);
      
  } else if (cmd_pr == 0) {
    //new process
    //    printf("new process\n");
    char* path = curr_cmd[0].args[0];
    execv(path, curr_cmd[0].args);
      
  } else {
    int cmd_pr_wait = wait(NULL);
    //    printf("this is the parent\n");
  }  
return;
}

void error_occured() {
char error_message[30] = "An error has occurred\n";
int nbytes_written = write(STDERR_FILENO, error_message, strlen(error_message));
if(nbytes_written != strlen(error_message)){
  exit(2);  // Should almost never happen -- if it does, error is unrecoverable
 }
}


