/*
* Author       : Shivani Sanjay Tergaonkar
* Email ID     : stergaonkar@scu.edu
* File Name    : main.c
* Date Created : 01/07/2021
* Version      : v1.0
* Description  : Write a shell that is similar to
*                Fig. 1-19 but contains enough code
*                that it actually works so you can test it.
*                You might also add some features such as
*                redirection of input and output, pipes,
*                and background jobs.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include<fcntl.h>
#define TRUE 1

// This function prints Shell command prompt
void type_prompt(){
  printf("<coen-283-shell> ");
}

// This function accepts a command string, and exits the program
//  if it is a "quit" command 
void is_quit_command(char *command){
  if (strcmp(command, "quit") == 0)
      exit(0);
}

// This function clears the scan buffer
void clear_buffer(){
  char temp;
  scanf("%c", &temp);
}

// This function accepts an argument array, and performs the following steps.
// a. Iterates over the argument array until the end of the string.
// b. If the entry in the argument array matches with with '<', '>' or '>>', the
//    entry after that is the file name, and is copied to the variable file_name.
// c. The entry '<', '>' or '>>' is replaced with NULL.
// d. Use the file I/O system call to open the file in read / write / append mode
void redirection_handler(char **args){
  int file_descriptor, i;
  char file_name[64];
  // Iterate until the entry in the argument array matches "\0"
  for(i=0; args[i] != NULL; i++) {
      if(strcmp(args[i],"<")==0){ // user has requested an input operation
        args[i]=NULL;
        strcpy(file_name,args[i+1]);
        file_descriptor = open(file_name, O_RDONLY, 0); // open the file in read only mode
        if (file_descriptor < 0) {
           // if the file operation has failed, print the error and exit
           perror("Couldn't open input file");
           exit(0);
        }
        // use the file_descriptor provided by the user to get the input
        dup2(file_descriptor, STDIN_FILENO);
        close(file_descriptor);
      } else if(strcmp(args[i],">")==0){
        // user has requested an output file operation
        args[i]=NULL;
        strcpy(file_name, args[i+1]);
        freopen(file_name, "w", stdout); // open the file in write only mode
      } else if(strcmp(args[i],">>")==0){
        // user has requested to append the output of the command to an existing file
        args[i] = NULL;
        strcpy(file_name, args[i+1]);
        freopen(file_name, "a", stdout); // open the existing file for appending new contents
      }
  }
}

// This function provides the user with a command prompt.
// a. If the user inputs "quit", then the program exits.
// b. Else, the parent process spawns a child process  and waits for the child process to terminate.
// c. The child process then invokes a system call 'execvp' to execute the user input command
void shell(){
  char command[10], *param, *args[20];
  int status;
  pid_t pid;

  while (TRUE) { // repeat forever
    type_prompt();

    scanf("%[^\n]", command); // Accepts user input command
    is_quit_command(command);
    clear_buffer(); // Clears the buffer content

    // Splits the user input command into different parameters
    param = strtok(command, " ");
    int j=1;
    args[0] = param;

    while(param != NULL){
     param = strtok(NULL, " ");
     args[j] = param;
     j++;
    }

    pid = fork();

    if(pid<0){
      // An error occured when fork() returns -1
      perror("fork failed");
      exit(EXIT_FAILURE);
    } else if (pid == 0) { // fork the child process when fork() returns 0
      redirection_handler(args); // Set up redirection in the child process
      execvp(args[0], args); // Execute the command using execvp system call
      perror("Error: ");
    } else {
      // In Parent process when fork() returns a positive number
      waitpid(-1, &status, 0); // wait for child to exit
    }

  }
}

// The main function used to invoke a long running shell function call
int main(int argc, char **argv){
  shell();
  return EXIT_SUCCESS;
}
