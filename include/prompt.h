#ifndef _PROMPT_H
#define _PROMPT_H

#include <stdbool.h>
#include "basic.h"

/*
 * clears prompt section
 */
public void prompt_clear(bool restore);

/*
 * shows a Y/N prompt with given message to the user, and returns result as an boolean
 */
public bool prompt_bool(char *message);

/*
 *	read a string from user, also we can show some message to user
 */
public char *prompt_string(char *message);

#endif
