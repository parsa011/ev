#ifndef _EDITOR_H
# define _EDITOR_H

#include "basic.h"

typedef enum {
	MODE_INSERT,
	MODE_LOCK,
	MODE_PROMPT
} editor_mode;

typedef struct {
	char *file_path;
	editor_mode mode;
} editor_t;

/*
 * init editor in raw mode and other startup configurations
 */
public void editor_init();

/*
 * reset shell to cooked mode and close statements, like save changed file
 * question and this kind of stuff
 */
public void editor_close();

/*
 * runs editor infinite loop, taking command and doing them
 */
public return_message editor_run();

/*
 * opens given file path into our gloabl editor variable 'editor'
 */
public return_message editor_file_open(char *file_name);

/*
 * close current file of editor and opens new empty buffer
 */
public return_message editor_file_close();

/*
 * save buffer into current open file in 'editor'
 */
public return_message editor_file_save();

#endif
