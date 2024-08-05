# UNIX Command Line Interpreter (CS480_p2) project
A basic command line interpreter for UNIX, focusing on user input parsing and process management.

## About CS480_p2 project
Acts as simple command line interpreter for UNIX system. 
This project is built as the course project for CS480 Operating Systems at San Diego State University under the supervision of Dr. John Carroll. 

### Purpose
- This project is for understanding general operating systems, using UNIX programming such as fork(), exec(), pipe(), constricting a shell, interprocess communication, and concurrent programming.
- Throughout this project, I used a basic text editor (Vim) and debugged with gdb (the gnu debugger), which benefitted me in identifying and resolving issues in programs.

### Main function
- prompts input line with handling an empty line, processing the line, or terminating the program
- handles special metacharacters for redirection, pipelines, printing PID, comments, and escaped character words
- implemented history mechanism

### Description of files
- 1_p2.c - driver and source code for every function but the getword()
- 2_p2.h - header information for the C source files in the program
- 3_getword.c - source code file for the function getword(): splits and defines a word from input and returns the number of characters in the word 
- 4_getword.h - header file for the getword() function, CS480 course resource at San Diego State University
- 5_CHK.h - CS480 course resource at San Diego State University
- 6_makefile - makefile for this project, CS480 course resource at San Diego State University

## Authors
- Hyunhee Kwak hkwak9458(at)sdsu.edu
- Prof. John Carroll 

