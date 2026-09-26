#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>      // Added for fork(), execvp(), dup2(), close()
#include <sys/wait.h>    // Added for waitpid(), wait()
#include <fcntl.h>       // Added for open(), O_RDONLY, O_WRONLY, etc.

#include "param.hpp"
#include "parse.hpp"

/* text shown before each command is read */
static const char *PROMPT = "myshell$ ";

/*
 * Returns true if any command line argument is exactly "-Debug".
 */
static bool hasDebugFlag(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-Debug") == 0)
        {
            return true;
        }
    }
    return false;
}

/*
 * Returns true when the parsed command is a single word "exit".
 */
static bool isExitCommand(const Param &param)
{
    return (param.getArgumentCount() == 1) &&
           (strcmp(param.getArgumentVector()[0], "exit") == 0);
}

/*
 * Runs the prompt / read / parse loop.
 */
int main(int argc, char *argv[])
{
    bool debug = hasDebugFlag(argc, argv);

    char  *line   = NULL; 
    size_t bufLen = 0;    

    while (true)
    {
        // ZOMBIE PREVENTION: Clean up any background processes that finished 
        // since the last iteration without blocking the shell.
        while (waitpid(-1, NULL, WNOHANG) > 0);

        printf("%s", PROMPT);
        fflush(stdout);

        ssize_t count = getline(&line, &bufLen, stdin);
        if (count == -1)
        {
            printf("\n");
            break;
        }

        Param param;
        if (!parseCommand(line, param))
        {
            continue;
        }

        if (isExitCommand(param))
        {
            // EXIT REQUIREMENT: Wait for all running children to terminate before exiting
            while (wait(NULL) > 0);
            break;
        }

        if (param.getArgumentCount() == 0 &&
            param.getInputRedirect() == NULL &&
            param.getOutputRedirect() == NULL &&
            param.getBackground() == 0)
        {
            continue;
        }

        if (debug)
        {
            param.printParams();
        }

        // If there are no actual commands to execute (e.g., just a lone "&"), skip execution
        if (param.getArgumentCount() == 0)
        {
            continue;
        }

        // --- PART 2: PROCESS CREATION AND EXECUTION ---
        pid_t pid = fork();

        if (pid < 0) 
        {
            perror("myshell: fork failed");
        } 
        else if (pid == 0) 
        {
            // ---> CHILD PROCESS <---
            
            // 1. Input Redirection
            if (param.getInputRedirect() != NULL) 
            {
                int fd_in = open(param.getInputRedirect(), O_RDONLY);
                if (fd_in < 0) 
                {
                    perror("myshell: input redirection failed");
                    exit(1); // Exit child immediately if file doesn't exist/can't be opened
                }
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }

            // 2. Output Redirection
            if (param.getOutputRedirect() != NULL) 
            {
                // Open for writing, create if it doesn't exist, truncate to 0 if it does.
                // 0644 sets standard file permissions (rw-r--r--)
                int fd_out = open(param.getOutputRedirect(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out < 0) 
                {
                    perror("myshell: output redirection failed");
                    exit(1);
                }
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }

            // 3. Execute Command
            char *const *args = param.getArgumentVector();
            if (execvp(args[0], (char **)args) < 0) 
            {
                perror("myshell: execution failed");
                exit(1);
            }
        } 
        else 
        {
            // ---> PARENT PROCESS <---
            
            // Wait for the child if it is NOT a background process
            if (param.getBackground() == 0) 
            {
                waitpid(pid, NULL, 0);
            }
        }
    }

    free(line);
    return 0;
}