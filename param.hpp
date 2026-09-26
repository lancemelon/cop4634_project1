#ifndef PARAM_HPP
#define PARAM_HPP

#define MAXARGS 32

class Param {
private:
    char *inputRedirect;           /* file name or NULL */
    char *outputRedirect;          /* file name or NULL */
    int background;                /* either 0 (false) or 1 (true) */
    int argumentCount;             /* number of tokens in argument vector */
    char *argumentVector[MAXARGS]; /* array of strings */

public:
    // Constructor
    Param();

    // Getters and setters for the parser to use
    void setInputRedirect(char *file);
    void setOutputRedirect(char *file);
    void setBackground(int val);
    void addArgument(char *arg);
    
    // Part 2 will need this to pass to execvp
    char** getArgumentVector();

    // Print functionality for debugging
    void printParams();
};

#endif