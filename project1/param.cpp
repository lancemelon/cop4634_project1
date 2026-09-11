#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "param.hpp"

/*
 * Initializes an empty command: no redirection, foreground execution,
 * and an empty (NULL terminated) argument vector.
 */
Param::Param()
{
    inputRedirect  = NULL;
    outputRedirect = NULL;
    background     = 0;
    argumentCount  = 0;

    for (int i = 0; i <= MAXARGS; i++)
    {
        argumentVector[i] = NULL;
    }
}

/*
 * Releases every heap allocated string owned by the object.
 */
Param::~Param()
{
    free(inputRedirect);
    free(outputRedirect);

    for (int i = 0; i < argumentCount; i++)
    {
        free(argumentVector[i]);
        argumentVector[i] = NULL;
    }
}

/*
 * Stores a private copy of fileName as the input redirection target.
 * Any previously stored value is released first.
 */
void Param::setInputRedirect(const char *fileName)
{
    free(inputRedirect);
    inputRedirect = (fileName != NULL) ? strdup(fileName) : NULL;
}

/*
 * Stores a private copy of fileName as the output redirection target.
 * Any previously stored value is released first.
 */
void Param::setOutputRedirect(const char *fileName)
{
    free(outputRedirect);
    outputRedirect = (fileName != NULL) ? strdup(fileName) : NULL;
}

/*
 * Records whether the command should run in the background.
 */
void Param::setBackground(int flag)
{
    background = (flag != 0) ? 1 : 0;
}

/*
 * Appends a private copy of token to the argument vector and keeps the
 * vector NULL terminated. Returns false (and adds nothing) if the
 * vector is already full.
 */
bool Param::addArgument(const char *token)
{
    /* leave room for the trailing NULL entry */
    if (argumentCount >= MAXARGS)
    {
        return false;
    }

    argumentVector[argumentCount] = strdup(token);
    argumentCount++;
    argumentVector[argumentCount] = NULL;
    return true;
}

const char *Param::getInputRedirect() const
{
    return inputRedirect;
}

const char *Param::getOutputRedirect() const
{
    return outputRedirect;
}

int Param::getBackground() const
{
    return background;
}

int Param::getArgumentCount() const
{
    return argumentCount;
}

char *const *Param::getArgumentVector() const
{
    return argumentVector;
}

/*
 * Prints the parsed command in a readable form. Called only when the
 * shell is started with the -Debug option.
 */
void Param::printParams() const
{
    printf("---------------------------------------------\n");
    printf("input redirect:  %s\n",
           (inputRedirect != NULL) ? inputRedirect : "NULL");
    printf("output redirect: %s\n",
           (outputRedirect != NULL) ? outputRedirect : "NULL");
    printf("background:      %d\n", background);
    printf("arg count:       %d\n", argumentCount);

    for (int i = 0; i < argumentCount; i++)
    {
        printf("  arg[%d]: %s\n", i, argumentVector[i]);
    }
    printf("---------------------------------------------\n");
}
