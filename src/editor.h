#ifndef _EDITOR_H
# define _EDITOR_H

#include "basic.h"

typedef struct {
	char *file_path;
} editor_t;

/*
 * init editor in raw mode and other startup configurations
 */
void editor_init();

/*
 * reset shell to cooked mode and close statements, like save changed file
 * question and this kind of stuff
 */
void editor_close();

/*
 * opens given file path into our gloabl editor variable 'editor'
 */
return_message editor_file_open(char *file_name);

/*
 * close current file of editor and opens new empty buffer
 */
return_message editor_file_close();

/*
 * save buffer into current open file in 'editor'
 */
return_message editor_file_save();

#endif
