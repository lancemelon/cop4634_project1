#ifndef PARSE_HPP
#define PARSE_HPP

#include "param.hpp"

/*
 * Breaks the raw input line into whitespace delimited tokens and sorts
 * them into the fields of param:
 *
 *   <file   -> input redirection  (stored via setInputRedirect)
 *   >file   -> output redirection (stored via setOutputRedirect)
 *   &       -> background flag    (stored via setBackground)
 *   other   -> appended to the argument vector
 *
 * The line buffer is modified in place (strtok). param is assumed to be
 * a freshly constructed, empty object.
 *
 * Returns true on success, or false if a syntax problem was reported
 * (for example, a redirection operator with no file name).
 */
bool parseCommand(char *line, Param &param);

#endif
