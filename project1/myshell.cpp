#include <cstdio>
#include <cstdlib>
#include <cstring>

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
    }

    free(line);
    return 0;
}
