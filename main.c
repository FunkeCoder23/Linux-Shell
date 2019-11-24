// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:Aaron Tobias, Matt Stran, Molly Kendrick Date: 11/19/2019

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_ARGS	64
#define MAX_ARG_LEN	16
#define MAX_LINE_LEN	80
#define WHITESPACE	" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Help function */
void help()
{
  int line=0;
  int page=0;
  char *c; //continue char
  char* helpText[80]; //helpText buffer


  helpText[0]= "DESCRIPTION";
  helpText[1]= "\thelp is a manual that describes and lists the functions and commands";
  helpText[2]= "\tused in LINUX command line. Commands are case sensitive.\r\n";
  helpText[3]= "COMMANDS";
  helpText[4]= "\tE comment - echo, display comment on screen followed by a new line.";
  helpText[5]= "\tif no argument, simply issue a new prompt";
  helpText[6]= "\r\n";
  helpText[7]= "\tC file1 file2 - copy, create file2 and copy all bytes of file1 to";
  helpText[8]= "\tfile2 without deleting file1. ";
  helpText[9]= "\r\n";
  helpText[10]= "\tL - list directory contents.";
  helpText[11]= "\r\n";
  helpText[12]= "\tH - help, display this user manual";
  helpText[13]= "\r\n";
  helpText[14]= "\tP file - print, display the contents of the file named.";
  helpText[15]= "\r\n";
  helpText[16]= "\tD file - delete file named.";
  helpText[17]= "\r\n";
  helpText[18]= "\tM file - make, create the named text file by launching a text editor.";
  helpText[19]= "\r\n";
  helpText[20]= "\tQ - quit the shell.";
  helpText[21]= "\r\n";
  helpText[22]= "\tS - surf the web by launching a browser as a background process.";
  helpText[23]= "\r\n";
  helpText[24]= "\tW - wipe, clear the screen.";
  helpText[25]= "\r\n";
  helpText[26]= "\tX program - execute the named program.";
  helpText[27]= "\r\n";

    for (line=0; helpText[line]!=0; line++)	//print each line
      printf("%s \r\n",helpText[line]);
  return;
}




/*  End of Help Function */

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while (1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      //...
      parseCommand(cmdLine, &command);
      //...
      command.argv[command.argc] = NULL;
      //printf("command is %s\r\n",command.name);
      //printf("argv is %s\r\n",*argv);
      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp(command.name, command.argv);
      }
      /* Wait for the child to terminate */
      wait(&status);
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

int shellCmd(struct command_t *cmd) 	//custom function that determines the command
{											//entered and executes the function of that command
  int pid,status;
  char* args[4];
  
  if(!strcmp(cmd->name, "C")) 			//copy file1 to a new file, file2
  {
    cmd->name="cp";
  }  
  else if(!strcmp(cmd->name, "D"))	//delete file entered
  {
    cmd->name="rm";
  }
  else if(!strcmp(cmd->name, "E"))	//print comment to screen
  {
    if(cmd->argv[2]!=0)
    {
    cmd->argv[0]="echo";
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp("echo", cmd->argv);		//send the comment
      }
      /* Wait for the child to terminate */
      wait(&status);

    args[0]="echo";
    args[1]="\r\n";
    args[2]=0;
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp("echo", args);				//send a new line
      }
      /* Wait for the child to terminate */
      wait(&status);
    }
    
  }
  else if(!strcmp(cmd->name, "H")) 	//print custom help file
  {
    help();
  }
  else if(!strcmp(cmd->name, "L")) 	//list all files in working directory
  {  										//in long format
     if ((pid = fork()) == 0) {
    /* Child executing command */		
      args[0]="echo";	
      args[1]="\r\n";
      args[2]=0;
      execvp("echo",args);				//send a new line
    }
    /* Wait for the child to terminate */
    wait(&status);
    
    if ((pid = fork()) == 0) {
    /* Child executing command */
     args[0]="pwd";
     args[1]=0;
     execvp("pwd",args);				//send the working directory
    }
    /* Wait for the child to terminate */
    wait(&status);

    if ((pid = fork()) == 0) {
    /* Child executing command */
      args[0]="echo";
      args[1]="\r\n";
      args[2]=0;
      execvp("echo",args);				//send a new line
    }
    /* Wait for the child to terminate */
    wait(&status);

    if ((pid = fork()) == 0) {
    /* Child executing command */
     args[0]="ls";
     args[1]="-l";
     args[2]=0;
     execvp("ls",args);					//send the long format argument
    }
    /* Wait for the child to terminate */
    wait(&status);

  }
  else if(!strcmp(cmd->name, "M"))	//create a named text file with a text editor
  {
    cmd->name="nano";
  }
  else if(!strcmp(cmd->name, "P"))	//print contents of the file entered
  {
    cmd->name="cat";
  }
  else if(!strcmp(cmd->name, "Q"))	//exit the shell with return status 1
  {
    exit(1);
  }
  else if(!strcmp(cmd->name, "S"))	//launch internet browser (firefox)
  {
    cmd->name="firefox";
  }
    else if(!strcmp(cmd->name, "W"))	//clear/wipe the screen
  {
    cmd->name="clear";
  }
    else if(!strcmp(cmd->name, "X"))	//run the program entered
  {
    cmd->name=cmd->argv[1];
    for(int i = 1; cmd->argv[i+1];i++)
    {
      cmd->argv[i]=cmd->argv[i+1];
    }

  }
  return 1;
}

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);

   shellCmd(cmd);	//custom function that determines the command entered
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
    char* promptString = "linux adt-mss-mek: ";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */
