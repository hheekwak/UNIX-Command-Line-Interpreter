/**
 * Program #4 - p2.c 
 * Description: The program imitates a simple command line interpreter for our UNIX system(p2), and several enhancements like pipe line use and dealing with meta characters, "#" and "!" are added(p4)  
 * CS480 Professor John Carroll
 * Date 11/30/2022 (12/05/2022)
 * @author Hyunhee Kwak
 */

#include "p2.h"

int Back = 0;		   // Flag for "&"
int Flag_sp_in = 0;	// Flag for "<" 
int Flag_sp_out = 0;	// Flag 1 for ">", 2 for ">&", 3 for ">>" and 4 for ">>&"
int Flag_end = 0;	   // Flag for termination and ".end"
int Flag_pipe = 0;	// Flag for pipe
int noinput = 0;     // Flag if input exists for p4

char *file_p_in = NULL;    // file name to be saved
char *file_p_out = NULL;

int i;			         // newargv index (word count) 
char lilBuff[STORAGE];  // to filter "!!"
int pipepoint = 0;   	// p4 index of after pipe
int commandnum = 0;     // p4 for prompt counter and history
int s = 0;		         // p4 next read-in spot
char history[10][STORAGE * MAXITEM];   // p4 command history saved
char lastword[STORAGE]; // p4 for "!$"

void parse(char bigBuff[], char *newargv[])
{
   int c;			      // character count 
   char *p = bigBuff;	// points the next writing spot    
   char *e = lilBuff;   // temporary use
   int Flag_hash = 0;   // p4 for "#"
   noinput = 0;         // initialization
	
   if (commandnum > 1 && i > 0)        // p4 when "last word" exists 
   {
      strcpy(lastword,newargv[i - 1]);	// p4 save the last word of last command
   }
		
   history[commandnum][0] = '\0'; 		// p4 assist to break loop when no input left
   fgets(history[commandnum], (STORAGE * MAXITEM), stdin);	// p4 read the entire line 

   i = 0;			      //initialization

   if(history[commandnum][0] == '\n')
   {
      noinput = 1;
      return;
   } 
   if(history[commandnum][0] == '\0')
   {       
      Flag_end = 1;
      return;
   }
   for(;;)
   {
      if (i == 0 )
      {
         c = getword(e);
      
         if((c == -1) || strcmp(e, "end.") == 0)
         {
            Flag_end = 1;
            break;
         }
      	
         if (strcmp(e, "!!") == 0)     // repeat the last command
         {
            strcpy(history[commandnum], history[commandnum - 1]);
            s = 0;
            continue;
         }		
         else if (lilBuff[0] == '!')   // p4 call command from history
         {
            strcpy(history[commandnum], history[atoi(e + 1)]);
            s = 0;
            continue;
         }
         else
         {
            memset(newargv, '\0', sizeof(newargv));
            strcpy(p, e);
         }
      }
      else
      {
         c = getword(p);
      }
   
      if (c == 1 && strcmp(p, "#") == 0)
      {
         Flag_hash = 1;
      }
   
      if (strcmp(p, "&") == 0) 
      {
         Back = 1;
         e = p;		            // to remember "&"
         p = p + c + 1;
         continue;
      }
      else if(strcmp(p,"\0") == 0)	 
      {
         c = 0;	
         Back = Back;
      }
      else
      {
         if(Back == 1)
         {
            newargv[i] = e;		// "&" to newargv[i]
            i++;
         }
         Back = 0;
      }
   
   
      if ((strcmp(p, "<") == 0))
      {
         p = p + c + 1;
         c = getword(p);
         file_p_in = p;		      // save the file name
         p = p + c + 1;
         Flag_sp_in = 1;	   	// input file flag on
         noinput = 0;
         continue; 
      }
      if ((strcmp(p, ">") == 0))
      {
         p = p + c + 1;
         c = getword(p);
         file_p_out = p;
         p = p + c + 1;
         Flag_sp_out = 1;        // output file flag on
         noinput = 0;
         continue;
      }
      else if ((strcmp(p, ">&") == 0))
      {
         p = p + c + 1;
         c = getword(p);
         file_p_out = p;
         p = p + c + 1;
         Flag_sp_out = 2;
         noinput = 0;
         continue;
      }
      else if ((strcmp(p, ">>") == 0))
      {       
         p = p + c + 1;
         c = getword(p);
         file_p_out = p;
         p = p + c + 1;
         Flag_sp_out = 3;
         noinput = 0;       
         continue;
      }
      else if ((strcmp(p, ">>&") == 0))
      {       
         p = p + c + 1;
         c = getword(p);
         file_p_out = p;
         p = p + c + 1;
         Flag_sp_out = 4;
         noinput = 0;       
         continue;
      }
   
      if ((strcmp(p, "|") == 0))
      {
         Flag_pipe = 1;
         newargv[i] = NULL;
         i++;
         pipepoint = i;
         p = p + c + 1;
         noinput = 0;
         continue;
      }
   
      if (strcmp(p, "!$") == 0)
      {       
         newargv[i] = lastword;
         i++;
         continue;
      }
   
      if (c == 0)
      {
         newargv[i] = NULL;
         break;
      }
   	
      if (Flag_hash == 0)	   // when it is not a acomment, added to newargv
      {
         newargv[i] = p;		
         i++;
      }
      p = p + c + 1;          // 1 is for space of null character 
   }
}

void sigHandler(int signum)
{
}

int main(int argc,char **argv)
{
   char bigBuff[STORAGE * MAXITEM];
   char *newargv[MAXITEM];
   pid_t kidpid, grandpid;

   int input_fd;
   int output_fd;

   int flags;
   int fildes[2];          // p4 pipeline use

   int argc_flag = 0;	   // p4 if argc is 2

   (void)signal (SIGTERM, sigHandler);

   setpgid(0,0);

   if(argc == 2)
   {
      argc_flag = 1;
   
      flags = O_RDONLY;
      if ((input_fd = open(argv[1], flags, S_IRUSR | S_IWUSR)) < 0)
      {
         perror("oops, open failed!");
         exit(1);
      }
      CHK(dup2(input_fd, STDIN_FILENO));
      CHK(close(input_fd));
   }

   for(;;)
   {
      Flag_sp_in = 0;		// reseting Flags, initialize pipepoint
      Flag_sp_out = 0;
      Flag_end = 0;
      Back = 0;
      Flag_pipe = 0;
      pipepoint = 0;
   	
      if (argc_flag == 0)	// p4 if no argv[1], print prompts
      {
         if (noinput == 1)
         {
            printf("%%%d%% ", (commandnum));
         }
         else
         {
            printf("%%%d%% ", (++commandnum));
            s = 0;
         }    
      }
   	
      parse(bigBuff, newargv);
   
      if (Flag_end == 1) 
         break;
         
      if (noinput == 1)
         continue;
   
      if ((newargv[0] != NULL) && (strcmp(newargv[0], "cd") == 0))
      {
         if (newargv[1] == NULL)
         {
            chdir(getenv("HOME")); 
         }
         else if(newargv[2] != NULL)
         {
            fprintf(stderr, "too many arguments for cd command\n");
            continue;
         }
         else
         {
            chdir(newargv[1]);
            continue; 
         }
      }
   
      fflush(stdout);
      fflush(stderr);
   
      if(Flag_pipe == 1)
      {
         if (-1 == (kidpid = (int) fork()))		// if fork was not successful
         {
            perror("Cannot fork");
            exit(EXIT_FAILURE);
         }
         else if (0 == kidpid)				      // child work
         {
            CHK(pipe(fildes));
            CHK(grandpid = fork ());
         
            if (0 == grandpid)			         // p4 grandchild work			
            {
               if(Flag_sp_in == 1)		         // handle input file
               {
                  flags = O_RDONLY;
                  if ((input_fd = open(file_p_in, flags, S_IRUSR | S_IWUSR)) < 0)
                  {
                     perror("oops, open failed!");
                     exit(1);
                  }
                  CHK(dup2(input_fd,STDIN_FILENO));
                  CHK(close(input_fd));
               }
               CHK(dup2(fildes[1], STDOUT_FILENO));
               CHK(close(fildes[0]));
               CHK(close(fildes[1]));
            
               if(execvp(newargv[0], newargv) < 0)	// when execvp failed
               {
                  perror("exec failed");
                  exit(EXIT_FAILURE);
               }
            }
            else					                     // child work
            {
               CHK(dup2(fildes[0], STDIN_FILENO));
            
               if (Flag_sp_out == 1)
               {	
                  flags = O_WRONLY | O_CREAT | O_EXCL; 
                  if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
                  {
                     perror("oops, open failed!");
                     exit(1);
                  }
                  CHK(dup2(output_fd, STDOUT_FILENO));
                  CHK(close(output_fd));
               }
               else if (Flag_sp_out == 2)
               {
                  flags = O_WRONLY | O_CREAT | O_EXCL;
                  if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
                  {
                     perror("oops, open failed!");
                     exit(1);
                  }
                  CHK(dup2(output_fd, STDOUT_FILENO));
                  CHK(dup2(output_fd, STDERR_FILENO));
                  CHK(close(output_fd));
               }
            
               if (Flag_sp_out == 3)	
               {       
                  flags = O_WRONLY | O_APPEND; 
                  if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
                  {       
                     perror("oops, open failed!");
                     exit(1);
                  }
                  CHK(dup2(output_fd, STDOUT_FILENO));
                  CHK(close(output_fd));
               }
               else if (Flag_sp_out == 4)
               {       
                  flags = O_WRONLY | O_APPEND;
                  if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
                  {       
                     perror("oops, open failed!");
                     exit(1);
                  }
                  CHK(dup2(output_fd, STDOUT_FILENO));
                  CHK(dup2(output_fd, STDERR_FILENO));
                  CHK(close(output_fd));
               }
            
            
               CHK(close(fildes[0]));
               CHK(close(fildes[1]));
            
               if(execvp(newargv[pipepoint], newargv + pipepoint) < 0) 	// when execvp failed
               {
                  perror("exec failed");		
                  exit(EXIT_FAILURE);
               }
            }
         }
         else						// parent
         {
            if (Back == 1)
            {
               printf("%s [%d]\n",newargv[0],kidpid);
               // /dev/null
            }
            else
            {
               for(;;)
               {
                  pid_t pid;
                  CHK(pid = wait(NULL));
                  if (pid == kidpid)
                  {
                     break;
                  }
               }
            }
         }
      }
      else		               // when no pipe
      {
         if (-1 == (kidpid = (int) fork()))              // fork was not successful
         {
            perror("Cannot fork");
            exit(EXIT_FAILURE);
         }
         else if (0 == kidpid)                           // child will work
         {
            if(Flag_sp_in == 1)
            {
               flags = O_RDONLY;
               if ((input_fd = open(file_p_in, flags, S_IRUSR | S_IWUSR)) < 0)
               {
                  perror("oops, open failed!");
                  exit(1);
               }
               CHK(dup2(input_fd,STDIN_FILENO));
               CHK(close(input_fd));
            }
         
            if (Flag_sp_out == 1)
            {
               flags = O_WRONLY | O_CREAT | O_EXCL; 
               if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
               {
                  perror("oops, open failed!");
                  exit(1);
               }
               CHK(dup2(output_fd, STDOUT_FILENO));
               CHK(close(output_fd));
            }
            else if (Flag_sp_out == 2)
            {
               flags = O_WRONLY | O_CREAT | O_EXCL;
               if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
               {
                  perror("oops, open failed!");
                  exit(1);
               }
               CHK(dup2(output_fd, STDOUT_FILENO));
               CHK(dup2(output_fd, STDERR_FILENO));
               CHK(close(output_fd));
            }
            else if (Flag_sp_out == 3)
            {
               flags = O_WRONLY | O_APPEND;
               if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
               {
                  perror("oops, open failed!");
                  exit(1);
               }
               CHK(dup2(output_fd, STDOUT_FILENO));
               CHK(close(output_fd));
            }
            else if (Flag_sp_out == 4)
            {
               flags = O_WRONLY | O_APPEND;
               if ((output_fd = open(file_p_out, flags, S_IRUSR | S_IWUSR)) < 0)
               {
                  perror("oops, open failed!");
                  exit(1);
               }
               CHK(dup2(output_fd, STDOUT_FILENO));
               CHK(dup2(output_fd, STDERR_FILENO));
               CHK(close(output_fd));
            }
         
            if(execvp(newargv[0], newargv) < 0)     // when execvp failed
            {
               perror("exec failed");
               exit(EXIT_FAILURE);
            }
         }
         else			// parent
         {
            if (Back == 1)
            {
               printf("%s [%d]\n",newargv[0],kidpid);
                    // /dev/null
            }
            else
            {
               for(;;)
               {
                  pid_t pid;
                  CHK(pid = wait(NULL));
                  if (pid == kidpid)
                  {
                     break;
                  }
               }
            }	
         }
      }
   }

   killpg(getpgrp(), SIGTERM);	      // Terminate any children that are still running.
   if (argc_flag == 0)
   {
      printf("p2 terminated.\n");
   }
   exit(0);	
}
