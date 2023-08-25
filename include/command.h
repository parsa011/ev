#ifndef _COMMAND_H
# define _COMMAND_H

#include "basic.h"

typedef struct {
	char *desc;
	char *key_codes;
	return_message (*func)(char **); /* function */
} command;

#define MAKE_COMMAND(desc, keys, func) ((command){desc, keys, func})

#define EMPTY_COMMAND MAKE_COMMAND("Empty Command", "[NULL]", NULL)

public command commands[];

/*
 * takes key string and finds a proper command for key(or keys) if it exists
 * also if there is commands that start with given str, will get more keys to find
 * also returns NULL command if there isn't any matched command with given str
 */
public command command_read(char *str);

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
