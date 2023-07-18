/**
 * Program #1 - getword.c 
 * Description: The program splits and defines a word from input and returns the number of characters in the word 
 * CS480 Professor John Carroll
 * Date 9/12/2022
 * @author Hyunhee Kwak
 */

#include "getword.h"
#include "p2.h"

extern int commandnum;     //p4 prompts and history
extern int s;              //p4 next read-in spot
extern char history[10][STORAGE * MAXITEM];

int getword(char *w) 
{
   int c = 0;					//track and count num of char in the current word
   int iochar;
   int i;
   for(i = 0; i < STORAGE; ++i)			//clean up the array before collecting a word
   {
      w[i] = 0;
   }

   while ((iochar = history[commandnum][s]) != '\0')
   {
      if (c == (STORAGE - 1))          //until num of char reached to Max wordsize, 254
      {
         return c;			//stop and split the word
      }
      else if (iochar == 32)			//char is space
      {
         if ( c == 0)			//when word has not started	
         {
            s++;
            continue;		   //do nothing, ready to get next char 
         }
         else
         {
            return c;		   //space after word: current word is over
         }
      }
      else if (strcmp(w, "end.") == 0) //program termination sign
      {
         return -1;
      }	
      else if (iochar == '<' || iochar == '|' || iochar == '&')  //metacharacters
      {
         if ( c == 0)
         {
            *(w + c++) = iochar;
            s++;
            return c;
         }
         else
         {
            return c;		//complete the word as is without the metacharacter 
         }
      }
      else if (iochar == '>')
      {
         if (c == 0)
         {
            *(w + c++) = iochar;
            s++;
            iochar = history[commandnum][s];
            if (iochar == '>')
            {
               *(w + c++) = iochar;
               s++;
               iochar = history[commandnum][s];
               if (iochar == '&')
               {
                  *(w + c++) = iochar;	//when word is ">>&"
                  return c;
               }
               else				         //when word is ">>" not followed by '&'
               {
                  return c;		      //complete a word as ">>" 
               }
            }
            else if (iochar == '&')
            {
               *(w + c++) = iochar;		//when word is ">&"
               return c;
            }
            else					         //when '>' is not followed by '>' or '&'
            {
               return c;			      //complete the word as ">" and start new word
            }	
         }
         else              //after a word, '>' detected
         {
            return c;		//complete the current word
         }
      }
      else if (iochar == '\n')	
      {
         if (c == 0)
         {      
            return 0;
         }
         else
         {
            return c;		//finish current word first and next will be "new line"
         }
      }
      else if (iochar == '\\')		
      {
         s++;
         iochar = history[commandnum][s];
         if (iochar == '\n')
         {
            return c;		//finish current word first
         }
         else
         {
            *(w + c++) = iochar;
         }
      }
      else
      {
         *(w + c++) = iochar;
      }
      s++;	
   }
   return -1;
}
