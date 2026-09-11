#ifndef PARAM_HPP
#define PARAM_HPP

#define MAXARGS 32

/*
 * Stores the result of parsing one command line:
 *   - inputRedirect  : name of a file to redirect stdin from, or NULL
 *   - outputRedirect : name of a file to redirect stdout to, or NULL
 *   - background     : 1 if the command ends with '&', otherwise 0
 *   - argumentCount  : number of real argument tokens collected
 *   - argumentVector : NULL-terminated array of argument strings
 *
 * All strings stored in the object are heap allocated (strdup) and are
 * released by the destructor.
 */
class Param
{
  private:
    char *inputRedirect;           
    char *outputRedirect;         
    int   background;              
    int   argumentCount;            
    char *argumentVector[MAXARGS + 1]; 

  public:
    Param();
    ~Param();

    void setInputRedirect(const char *fileName);
    void setOutputRedirect(const char *fileName);
    void setBackground(int flag);
    bool addArgument(const char *token);

    const char *getInputRedirect() const;
    const char *getOutputRedirect() const;
    int getBackground() const;
    int getArgumentCount() const;
    char *const *getArgumentVector() const;

    void printParams() const;
};

#endif
