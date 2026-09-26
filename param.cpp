#include <iostream>
#include <cstring>
#include "param.hpp"

using namespace std;

// Constructor initializes everything to empty/null states
Param::Param() {
    inputRedirect = nullptr;
    outputRedirect = nullptr;
    background = 0;
    argumentCount = 0;
    for (int i = 0; i < MAXARGS; i++) {
        argumentVector[i] = nullptr;
    }
}

// Setters for parsed data
void Param::setInputRedirect(char *file) {
    inputRedirect = strdup(file);
}

void Param::setOutputRedirect(char *file) {
    outputRedirect = strdup(file);
}

void Param::setBackground(int val) {
    background = val;
}

void Param::addArgument(char *arg) {
    if (argumentCount < MAXARGS - 1) { 
        argumentVector[argumentCount++] = strdup(arg);
        argumentVector[argumentCount] = nullptr; // Null-terminate for execvp
    }
}

char** Param::getArgumentVector() {
    return argumentVector;
}

// Prints the object data if debug flag is active
void Param::printParams() {
    cout << "InputRedirect: [" << (inputRedirect ? inputRedirect : "NULL") << "]" << endl;
    cout << "OutputRedirect: [" << (outputRedirect ? outputRedirect : "NULL") << "]" << endl;
    cout << "Background: [" << background << "]" << endl;
    cout << "ArgumentCount: [" << argumentCount << "]" << endl;
    for (int i = 0; i < argumentCount; i++) {
        cout << "ArgumentVector[" << i << "]: [" << argumentVector[i] << "]" << endl;
    }
}