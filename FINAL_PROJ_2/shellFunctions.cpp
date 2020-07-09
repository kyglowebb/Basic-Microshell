/* Project Information */
//this project allows to create a smiple shell to excute files and make pipes between processes

/* Assignment 2: MicroShell
   Names: Kylee Webb, Martin Philip
   Class: CS 570, Summer 2020 
   Class Accounts: CSSC 2159, CSSC 2126
   Emails: Kawebb@sdsu.edu, martin.phillip@gcccd.edu */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include "shellFunctions.h"

int n = 0; /* number of calls to 'command' */
char* args[512];
pid_t pid;
int command_pipe[2];


char line[1024];
 void waiting(int temp_n)
{
   int i;
   for (i = 0; i < temp_n; ++i)
       wait(NULL);
}
/* Creating Processes & Piping & Forking */
 int command(int input, int first, int last)  
{
   int fd[2];                         // There will be a read in & a write end hence the 2 
    int flag=0;
   pipe( fd );                        // creates a pipe between the fd[0] and fd[1]
   pid = fork();                      // creates a child process

    if (pid == 0) {
       for(int i=0;args[i]!=0;i++)    
       {
          if(args[i][0]=='>')                                               // The i starts w/ 1st element of array 
                {                                                           // Then incrs; the 0 is the 1st char of 1st ele until incr
                   fd[1]=open(args[i+1], O_CREAT|O_TRUNC|O_WRONLY, 0644);   // The read end is opened fd[0]
                   flag=1;                                                  // Created by open, the file creation flag, write only.
                }                                                           
            if(args[i][0]=='>>' )                                           // added[0] 
                {
                   fd[1]=open(args[i+1], O_APPEND|O_WRONLY, 0644);          // The write end is opened fd[1]
                   flag=1;                                                  // File opened in append mode, write only.
                }                                                           
       }
       if (first == 1 && last == 0 && input == 0) {                         // keep track of order
           //fprintf(stderr, "here\n");
           dup2( fd[1], 1 );
       } else if (first == 0 && last == 0 && input != 0) {                  // check conditions for input 
           dup2(input, 0);
           dup2(fd[1], 1);
       } else {
           if(flag==1)
            {
               dup2(fd[1],1);
            }
           dup2( input, 0 );
       }                                                                    // finish the piping process and close up
         if(flag==1)
       {
                if(strcmp(args[1],">>")==0)                                 // if arg[1] == to char ">>" is True then
                   execlp(args[0], args[0], 0, (char *)0);                  // use int execlp(const char *file, const char *arg, ...);
                else if(strcmp(args[1],">")==0)                             // if arg[1] == to char ">" is True then
                   execlp(args[0], args[0], 0, (char *)0);                  // use int execlp(const char *file, const char *arg, ...);
                else
                   execlp(args[0], args[0], args[1], (char *)0);
       }
       else if (execvp( args[0], args) == -1 && args[0]=="ls"){             // Else if there are no elements within args; exit
           exit(1);
            }else if (access(args[0],X_OK) )
            {
             std::cout<<"Error: Not an executable file. Please try a different file."<< args[0]<<std::endl;
        exit(1);}else if (execv( args[0], args) == -1){ exit(1);}               // Even if it points to a filename but there are no args; exit
        }
  

   if (input != 0)                              // If input does not have anything else in it, we are done and can close out. 
       close(input);
    close(fd[1]);                               // We close the write end portion of the pipe
    if (last == 1)                              // If last flag is 1 
       close(fd[0]);                            // We close the read in portion of the pipe because no more input
   return fd[0];                                // We return the read in portion
}
/* Skips each space */
 char* skipspace(char* s)     
{
   while (isspace(*s)) ++s;     // Checks if it is a space then incr then returns 
   return s;
}

/* splits up each word into tokens*/
 void tokenize(char* cmd)
{
   cmd = skipspace(cmd);            // Skips white spaces
   char* next = strchr(cmd, ' ');   // String compares the cmd with ' "
   int i = 0;

   while(next != NULL) {            
       next[0] = '\0';
       args[i] = cmd;
       ++i;                         // Pre-increment i to next word
       cmd = skipspace(next + 1);   // Skips the spaces
       next = strchr(cmd, ' ');
   }

   if (cmd[0] != '\0') {
       args[i] = cmd;
       next = strchr(cmd, '\n');    // Keep going until it hits the null term.
       next[0] = '\0';
       ++i;
   }

   args[i] = NULL;                  
}



 int run(char* cmd, int input, int first, int last)
{
   tokenize(cmd);
   if (args[0] != NULL) {
       if (strcmp(args[0], "exit") == 0)            // compares chars starting at args[0] to exit & once each char == exit is T it exits
           exit(0);
       n += 1;
       return command(input, first, last);          // return command which is the forking and piping process
   }
   return 0;
}

int dowork(){

   printf("Microshell: Type 'exit' to exit.\n");
   while (1) {
       /* Print the command prompt */
       std::cout<<"CSSC2126% ";
       fflush(NULL);

       /* 0 a command line */
       if (!fgets(line, 1024, stdin))
           return 0;

       int input = 0;                               // We are going to use this to store the input
       int first = 1;

       char* cmd = line;
       char* next = strchr(cmd, '|');               // Find the first '|' within the input

       while (next != NULL) {                       // While there is no null value, keep searching for the other commands. 
           *next = '\0';                            // Next points to the '|' 
           input = run(cmd, input, first, 0);       // Store into input, first == 1, last == 0.

           cmd = next + 1;                          // Increment next to strchr the next portion of the input from user
           next = strchr(cmd, '|');                 // Find the next '|'
           first = 0;                               // Decrement first to 0                   
       }
       input = run(cmd, input, first, 1);           // When next == NULL, store into input first == 0 and last == 1.
       waiting(n);
       n = 0;
   }

    return 0;
}
