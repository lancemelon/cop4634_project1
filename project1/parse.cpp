#include <cstdio>
#include <cstring>

#include "parse.hpp"
#include "param.hpp"

static const char *DELIMITERS = " \t\n";

/*
 * See parse.hpp.
 */
bool parseCommand(char *line, Param &param)
{
    char *token = strtok(line, DELIMITERS);

    while (token != NULL)
    {
        if (token[0] == '<')
        {
            if (token[1] == '\0')
            {
                fprintf(stderr,
                        "myshell: syntax error: expected file name after '<'\n");
                return false;
            }
            param.setInputRedirect(token + 1);
        }
        else if (token[0] == '>')
        {
            if (token[1] == '\0')
            {
                fprintf(stderr,
                        "myshell: syntax error: expected file name after '>'\n");
                return false;
            }
            param.setOutputRedirect(token + 1);
        }
        else if (strcmp(token, "&") == 0)
        {
            param.setBackground(1);
        }
        else
        {
            if (!param.addArgument(token))
            {
                fprintf(stderr,
                        "myshell: too many arguments (max %d)\n", MAXARGS);
                return false;
            }
        }

        token = strtok(NULL, DELIMITERS);
    }

    return true;
}
