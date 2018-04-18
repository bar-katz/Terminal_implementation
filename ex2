#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024
#define MAX_JOBS_SIZE 100

// background commands pid
int pid_background[MAX_JOBS_SIZE];

// background commands
char command_background[MAX_JOBS_SIZE][MAX_INPUT_SIZE];

// number of background commands
int background_size = 0;

/// remove job from list
/// \param pid pid of job
void removeFromJobs(int pid) {
  int rm_index = 0;
  int exists = 0;

  for(int i = 0; i < MAX_JOBS_SIZE; i++) {
    if(pid_background[i] == pid) {
      rm_index = i;
      exists = 1;
      break;
    }
  }

  if(exists) {
    for (int i = rm_index + 1; i < MAX_JOBS_SIZE; i++) {
      pid_background[i - 1] = pid_background[i];
      strcpy(command_background[i - 1], command_background[i]);
    }

    background_size--;
  }
}

/// handle signals
/// \param sig signal
void signal_handler(int sig) {

  // child terminated signal
  if(sig == SIGCHLD) {
    int state;

    // get pid of child
    int pid = wait3(&state, WNOHANG, (struct rusage *) NULL);

    // wait for child process that terminated for a clean exit
    waitpid(pid, &state, 0);

    // remove child from jobs array
    removeFromJobs(pid);
  }
}

/// tokenize the input to args
/// \param input user input
/// \param args tokenized command
/// \return number of args
size_t tokenize(char *input, char **args) {
  char *word;
  size_t args_size = 0;

  word = strtok(input, " \n");

  while (word != NULL) {

    strcpy(args[args_size], word);

    args_size++;
    word = strtok(NULL, " \n");
  }

  // free unused args
  for (int i = (int) args_size; i < MAX_INPUT_SIZE; i++) {
    free(args[i]);
    args[i] = NULL;
  }

  return args_size;
}

/// handles command
/// \param args command args
/// \param args_size number of args
/// \param background flag -- run command in background
void handleCommand(char **args, size_t args_size, int *background) {

  // handles most commands -- commands that can run in background
  if (strcmp(args[0], "") != 0 && strcmp(args[0], "exit") != 0 &&
      strcmp(args[0], "cd") != 0 && strcmp(args[0], "jobs") != 0) {

    // determine if command should run in background
    if (strcmp(args[args_size - 1], "&") == 0) {
      strcpy(args[args_size - 1], "");
      free(args[args_size - 1]);
      args[args_size - 1] = NULL;
      args_size--;
      *background = 1;
    }

    // fork -- handle parent and child
    int child_ret_val;
    int child_pid = fork();
    if (child_pid != 0) {
      // parent
      printf("%d\n", child_pid);

      if (!*background) {
        // command running in foreground -- wait for command to finish
        wait(&child_ret_val);
      } else {
        // command running in background -- add command to jobs array
        // add command pid
        pid_background[background_size] = child_pid;

        // add command
        for (int i = 0; i < args_size; i++) {
          strcat(command_background[background_size], args[i]);
          strcat(command_background[background_size], " ");
        }
        background_size++;
      }
    } else {
      // child
      // execute command
      int success;
      success = execvp(args[0], args);

      if (success == -1) {
        fprintf(stderr, "Error in system call.\n");
      }
    }
  }

  if (strcmp(args[0], "cd") == 0) {

    printf("%d\n", getpid());

    int success = chdir(args[1]);

    if (success == -1) {
      fprintf(stderr, "Error changing directory.\n");
    }
  }

  if (strcmp(args[0], "jobs") == 0) {

    printf("%d\n", getpid());

    for (int i = 0; i < background_size; i++) {
      printf("%d     %s\n", pid_background[i], command_background[i]);
    }
  }
}

int main() {
  char **args; // args for the command
  char input[MAX_INPUT_SIZE]; // user input
  char temp_command[MAX_INPUT_SIZE]; // temp command name
  int background = 0; // flag - run command in background or foreground

  // allocate space for command
  args = (char **) malloc(MAX_INPUT_SIZE * sizeof(char *));
  for (int i = 0; i < MAX_INPUT_SIZE; i++) {
    args[i] = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));
  }

  // handles child terminated signal
  signal(SIGCHLD, signal_handler);

  do {
    int stop_scan = 0;
    size_t args_size = 0;
    strcpy(temp_command, "");

    printf("prompt> ");

    // get command
    fgets(input, MAX_INPUT_SIZE, stdin);

    // blank command
    if(strcmp(input, "\n") == 0) {
      continue;
    }

    // get args and size of args from input
    args_size = tokenize(input, args);
    // save command name
    strcpy(temp_command, args[0]);

    // handle command
    handleCommand(args, args_size, &background);

    // restore args space for next command
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
      if(args[i] == NULL) {
        args[i] = (char *) malloc(MAX_INPUT_SIZE * sizeof(char));
      }
    }
  } while (strcmp(temp_command, "exit") != 0);

  // free allocated space
  for (int i = 0; i < MAX_INPUT_SIZE; i++) {
    free(args[i]);
  }
  free(args);

  // kill all background jobs
  for(int i = 0; i < background_size; i++) {
    removeFromJobs(pid_background[i]);
  }
}
