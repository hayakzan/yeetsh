/*
  
   yeetsh, a personal learning project on shells

*/

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Global variables */
char shell_paths[MAX_ENTRIES_IN_SHELLPATH][MAX_CHARS_PER_CMDLINE];
static char prompt[] = "yeetsh> "; /* Command line prompt */
static char *default_shell_path[2] = {"/bin", NULL};
/* End Global Variables */

struct Command {
  char** args;      
  char* outputFile;
};


char** tokenize_command_line(char* cmdline);
struct Command parse_command(char** tokens);
void eval(char** cmd_tokens);
int try_exec_builtin(struct Command** cmd);
void exec_external_cmd(struct Command* cmd);

/* Main REPL: read, evaluate, and print. */

int main() {
  set_shell_path(default_shell_path);

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
  return tokens;
}

/* Turn tokens into a command. */

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
  free(commands);
}

/* Execute built-in commands */

int try_exec_builtin(struct Command** cmd) {
  struct Command* curr_cmd = (struct Command*)malloc(sizeof(struct Command));
  char* exitMsg = "exit";
  char* cdMsg = "cd";
  //char* pathMsg = "path";
  char s[100];
  int ret;

  for (int i = 0; i < 2; i++) { 
    curr_cmd = (*cmd);   
  } 
    if (curr_cmd[0].args[0] == NULL) {
      printf("input is NULL\n");
      return 1;
    }
    else if (strcmp(curr_cmd[0].args[0], exitMsg) == 0) {
      printf("Have a good one.\n");
      exit(0);
    }

    else if (strcmp(curr_cmd[0].args[0], cdMsg) == 0) { //TODO: error if there are > 2 args after cd command
      ret = chdir(curr_cmd[1].args[0]);
      //print current dir
      printf("current dir: %s\n", getcwd(s, 100));

      if (ret != 0) {
	perror("!? yeetsh!");
      }
    }
  return 0;
}


/* Execute an external command */
void exec_external_cmd(struct Command* cmd) {
  (void)cmd;
  return;
}

void error_occured() {
char error_message[30] = "An error has occurred\n";
int nbytes_written = write(STDERR_FILENO, error_message, strlen(error_message));
if(nbytes_written != strlen(error_message)){
  exit(2);  
 }
}

