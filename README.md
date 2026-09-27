Lance Penaflor and Jay McIntosh
COP 4634: Systems & Networks
Project 1: Processes (myshell)

# COP 4634 Shell Project

Setup:
    1. Download and unzip the file from submission, or clone the repository to the department's public Linux server.
    2. The directory should contain these project files:
        - myshell.cpp - The main source code handling the shell loop, process creation, and redirection.
        - parse.cpp - Creates the command line parser.
        - parse.hpp - Defines the parser functionality.
        - param.cpp - Creates the Param class for storing parsed token data.
        - param.hpp - Defines the Param class structure.
        - Makefile - Script for compiling and cleaning the project.
        - README - This file.

Compiling:
    3. The project includes a Makefile for easy compilation. Open a terminal in the project directory and run:
         make

This should create an executable file named 'myshell'.

Running the program:
    4. After compiling, run the program by executing:
         ./myshell
       
       To run the shell in debug mode (which prints the parsed structure of each command), run:
         ./myshell -Debug

## Part 1
The first half of the project creates a command line parser. It reads a line of input using getline(3), splits it into tokens using strtok(3), and maps them into a Param object. It correctly identifies standard arguments, input/output redirection operators (< and >), and the background execution operator (&). Running the shell with the -Debug flag will print out the contents of the Param object after every command is entered to verify the parsing logic.

## Part 2
The second half extends the parser into a fully functional shell.
    - Process Creation: Commands are executed by forking a child process (fork(2)) and replacing its image using execvp().
    - Redirection: Input and output redirection is handled securely using freopen(3C), routing stdin and stdout to the specified files.
    - Background Processing: Background tasks (ending in &) run concurrently without blocking the prompt. Zombie processes are actively prevented using a non-blocking waitpid(2) sweep at the start of every loop.
    - Safe Exit: Typing exit ensures the shell waits for all child processes to terminate before closing.

Output & Interaction:
    5. Once running, the program will display the prompt myshell$ and wait for user input. You can test it with standard Linux commands:
         - ls -l (Basic execution)
         - ls -l >testfile.txt (Output redirection)
         - cat <myshell.cpp (Input redirection)
         - ./slow & (Background execution)
         - exit (Terminates the shell cleanly)

***Notes:
    - Environment: Tested on the department's public Linux servers and designed for a POSIX-compliant environment.
    - Syntax Requirement: Redirection symbols must be directly adjacent to the filename with no spaces (e.g., ls >out.txt, NOT ls > out.txt).