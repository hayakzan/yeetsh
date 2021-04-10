/*
  
   yeetsh, based on utcsh

*/

/* Read the additional functions from util.h. They may be beneficial to you
in the future */
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Global variables */
/* The array for holding shell paths. Can be edited by the functions in util.c*/
char shell_paths[MAX_ENTRIES_IN_SHELLPATH][MAX_CHARS_PER_CMDLINE];
static char prompt[] = "yeetsh> "; /* Command line prompt */
static char *default_shell_path[2] = {"/bin", NULL};
/* End Global Variables */

/* Convenience struct for describing a command. Modify this struct as you see
 * fit--add extra members to help you write your code. */
struct Command {
  char** args;      /* Argument array for the command */
  char* outputFile; /* Redirect target for file (NULL means no redirect) */
};

/* Here are the functions we recommend you implement */

char** tokenize_command_line(char* cmdline);
struct Command parse_command(char** tokens);
void eval(char** cmd_tokens);
int try_exec_builtin(struct Command** cmd);
void exec_external_cmd(struct Command* cmd);

/* Main REPL: read, evaluate, and print. This function should remain relatively
   short: if it grows beyond 60 lines, you're doing too much in main() and
   should try to move some of that work into other functions. */
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
    /* Print (optional) */
    free(line);
  }
  return 0;
}

/* NOTE: In the skeleton code, all function bodies below this line are dummy
implementations made to avoid warnings. You should delete them and replace them
with your own implementation. */

/** Turn a command line into tokens with strtok
 *
 * This function turns a command line into an array of arguments, making it
 * much easier to process. First, you should figure out how many arguments you
 * have, then allocate a char** of sufficient size and fill it using strtok()
 */
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

/** Turn tokens into a command.
 *
 * The `struct Command` represents a command to execute. This is the preferred
 * format for storing information about a command, though you are free to change
 * it. This function takes a sequence of tokens and turns them into a struct
 * Command.
 */

struct Command parse_command(char** tokens) {
  char* output = NULL;
  output = tokens[0];
  
  //Aggregate initialization 
   struct Command rtn = {.args = tokens,
  			.outputFile = output};
  return rtn;
}


/** Evaluate a single command
 *
 * Both built-ins and external commands can be passed to this function--it
 * should work out what the correct type is and take the appropriate action.
 */
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

/** Execute built-in commands
 *
 * If the command is a built-in command, immediately execute it and return 1
 * If the command is not a built-in command, do nothing and return 0
 */

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


/** Execute an external command
 *
 * Execute an external command by fork-and-exec. Should also take care of
 * output redirection, if any is requested
 */
void exec_external_cmd(struct Command* cmd) {
  (void)cmd;
  return;
}

void error_occured() {
char error_message[30] = "An error has occurred\n";
int nbytes_written = write(STDERR_FILENO, error_message, strlen(error_message));
if(nbytes_written != strlen(error_message)){
  exit(2);  // Should almost never happen -- if it does, error is unrecoverable
 }
}

