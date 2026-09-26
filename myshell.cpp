#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>      /* fork, execvp */
#include <sys/wait.h>    /* waitpid, wait */

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
        /* reap any background processes that have terminated */
        while (waitpid(-1, NULL, WNOHANG) > 0);

        printf("%s", PROMPT);
        fflush(stdout);

        ssize_t count = getline(&line, &bufLen, stdin);
        
        if (count == -1)
        {
            /* exit gracefully on end of file */
            printf("\n");
            break;
        }

        /* remove the trailing newline character */
        if (count > 0 && line[count - 1] == '\n') 
        {
            line[count - 1] = '\0';
        }

        /* skip empty input lines */
        if (strlen(line) == 0) continue;

        Param param;
        
        /* parse the input line; skip execution on syntax error */
        if (!parseCommand(line, param)) 
        {
            continue;
        }

        if (debug) 
        {
            param.printParams();
        }

        if (isExitCommand(param)) 
        {
            /* wait for all child processes to terminate before exiting */
            while (wait(NULL) > 0);
            break;
        }

        /* skip execution if no command was provided */
        if (param.getArgumentCount() == 0 &&
            param.getInputRedirect() == NULL &&
            param.getOutputRedirect() == NULL &&
            param.getBackground() == 0) 
        {
            continue;
        }

        /* ensure a command exists before attempting execution */
        if (param.getArgumentCount() == 0) 
        {
            continue;
        }

        /* create a new process to execute the parsed command */
        pid_t pid = fork();

        if (pid < 0) 
        {
            perror("myshell: fork failed");
        } 
        else if (pid == 0) 
        {
            /* child process */
            
            /* handle input redirection using freopen as required by part 2 */
            if (param.getInputRedirect() != NULL) 
            {
                if (freopen(param.getInputRedirect(), "r", stdin) == NULL) 
                {
                    perror("myshell: input redirection failed");
                    exit(1);
                }
            }

            /* handle output redirection using freopen as required by part 2 */
            if (param.getOutputRedirect() != NULL) 
            {
                if (freopen(param.getOutputRedirect(), "w", stdout) == NULL) 
                {
                    perror("myshell: output redirection failed");
                    exit(1);
                }
            }

            /* execute the parsed command */
            char *const *args = param.getArgumentVector();
            if (execvp(args[0], (char **)args) < 0) 
            {
                perror("myshell: execution failed");
                exit(1);
            }
        } 
        else 
        {
            /* parent process */
            
            /* wait for the foreground child process to complete */
            if (param.getBackground() == 0) 
            {
                waitpid(pid, NULL, 0);
            }
        }
    }

    free(line);
    return 0;
}