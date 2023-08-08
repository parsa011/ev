#ifndef _COMMAND_H
# define _COMMAND_H

#include "basic.h"

typedef struct {
	char *desc;
	char *key_codes;
	return_message (*func)(char **); /* function */
} command;

#define MAKE_COMMAND(desc, keys, func) ((command){desc, keys, func})

public command commands[];

/*
 * takes key from user until we find a proper command for key(or keys)
 */
public command command_read();

/*
 * finds a command with given pattern, if command's key_codes is null that means
 * if didnt found any command with given pattern
 */
public command command_get(char *pattern);

/*
 * returns true if we have any command that its key_codes start with (or is equal to the given pattern
 */
public bool command_exists(char *pattern);

/*
 * just prints command in human readable format
 */
public void command_print(command cmd);

#endif
