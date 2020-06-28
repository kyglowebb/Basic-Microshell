#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <istream>
#include <new> 

/* This program copies a file */

    // read file
    // determine if file is executable or not
    // if executable...
    //    create new process
    //    run program within process
    // if not print error message
    // if user types exit and hits return key must exit gracefully
    // need check if fully qualified path or needs to be searched for
    
// need to see if file is executable or shows error messages
// need to see if path is fully qualified or needs to be searched
// need to be able to create two proccesses with a pipe
    // "cssc9999% ls -l | sort -fi | grep msh.cpp”
    // 1st process sends output to second process
// need to exit gracefully



using namespace std;

// prototypes
void loop(void);
char *read_file();  // update parameters in 3 places
char **split_line(char *line);
int launch(char **args);    // get rid of ** below 
int num_builtins();
int cd(char **args);
int help(char **args);
void exit(char **args);
int execute(char **args);

void loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("cssc9999 ");
    line = read_file();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}

//function allocates a buffer to hold the line contents
// and returns the new line, the number of characters in 
// the line, and the size of the buffer.
char *read_file(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;
    lineSize = getline(&line, &len, stdin);
    printf("You entered %s, which has %zu chars.\n", line, lineSize - 1);
    
    
    return line;
    
}

#define split_BUFFSIZE 64
#define split_DELIM

// buffer and dynamically expanding it
// null-terminated array of pointers
char **split_line(char *line) {
    
    //int nb_dirs = 100;
    //char **dirs = malloc(sizeof(char*) * nb_dirs);
    
    int buffsize = split_BUFFSIZE, position = 0;
    
    // char **tokens = NULL;   // Pointer to a pointer initialized with null
    char **tokens = malloc(buffsize * sizeof(char*));
    //char **tokens = malloc(buffsize * sizeof(char*));
    // fix this
    char *token;
    
    if(!tokens) {       // if no tokens, error.
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, split_DELIM);  // returns a ptr to 1st token
    while(token != NULL) {
        tokens[position] = token;       // store each ptr in array(buffer) of char ptrs
        position++;
        
        if(position >= buffsize) {      // if not enough space, allocate more buff space.
            buffsize += split_BUFFSIZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, split_DELIM);  
    }
    tokens[position] = NULL;        // we have an array of tokens
    return tokens;
}

int launch(char **args) {   // takes in list of args
    pid_t pid, wpid; 
    int status;
    
    pid = fork();           // forks process & saves return value
    if(pid == 0) {          // child takes 1st if condition
        if(execvp(args[0], args) == -1) {    // child process
            perror("error");
        }
    exit(EXIT_FAILURE);
    }
    else if(pid < 0) {      // checks whether fork() had an error or not
        perror("error");
    }
    else {
        do {        // fork() executed successfully
            wpid = waitpid(pid, &status, WUNTRACED);
        }   while(!WIFEXITED(status) && !WIFISIGNALED(status));
    }
    
return 1;

}

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int(*built_func[]) (char **) = {
    &cd,
    &help,
    &exit
};

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

int cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "expected argument to \:cd\"\n");
    }
    else {
        if(chdir(args[1]) != 0) {
            perror("error");
        }
    }
    return 1;
}

int help(char **args) {
    
    int i;
    printf("First check");
    printf("Type the program names and arguments, and hit enter.\n");
    printf("The following are built in: \n");
    
    for (i = 0; i < num_builtins(); i++) {
        printf(" %s\n", builtin_str[i]);
    }
    
    printf("Use the man command for info");
    return 1;
}

void exit(char **args) {
   
    return 0;
}

int execute(char **args) {
    
    int i;
    
    if(args[0] == NULL) {
        return 1;           // empty command was entered 
    }
    
    for(i = 0; i < num_builtins(); i++) {
        if(strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return launch(**args);
}


// The first of these is a count of the arguments supplied
// to the program and the second is an array of pointers 
// to the strings which are those arguments
int main(int argc, char *argv[]) { 
    
	loop();
	return EXIT_SUCCESS;

    return 0;
}
