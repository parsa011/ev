#ifndef _PROMPT_H
#define _PROMPT_H

#include <stdbool.h>

/*
 * shows a Y/N prompt with given message to the user, and returns result as an boolean
 */
bool prompt_bool(char *message);

/*
 *	read a string from user, also we can show some message to user
 */
char *prompt_string(char *message);

#endif
