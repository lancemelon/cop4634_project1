#include <cstring>
#include "parse.hpp"

void parseLine(char* line, Param& paramObj) {
    // Tokenize by spaces, tabs, and newlines using strtok(3)
    char* token = strtok(line, " \t\n");
    
    while (token != nullptr) {
        // Check for input redirection
        if (token[0] == '<') {
            if (strlen(token) > 1) {
                paramObj.setInputRedirect(token + 1); // No space: <file
            } else {
                token = strtok(nullptr, " \t\n");     // Space: < file
                if (token) paramObj.setInputRedirect(token);
            }
        }
        // Check for output redirection
        else if (token[0] == '>') {
            if (strlen(token) > 1) {
                paramObj.setOutputRedirect(token + 1); // No space: >file
            } else {
                token = strtok(nullptr, " \t\n");      // Space: > file
                if (token) paramObj.setOutputRedirect(token);
            }
        }
        // Check for background operator
        else if (strcmp(token, "&") == 0) {
            paramObj.setBackground(1);
        }
        // Standard command or argument
        else {
            paramObj.addArgument(token);
        }
        
        // Grab the next token
        token = strtok(nullptr, " \t\n");
    }
}