#ifndef _BUFFER_H
# define _BUFFER_H

#include "../llink/llist.h"
#include "basic.h"
#include "line.h"
#include "editor.h"

typedef enum {
	MODE_INSERT,
	MODE_LOCK,
	MODE_PROMPT
} buffer_mode;

typedef enum {
	APPEND,
	REPLACE
} line_load_mode;

struct buffer {
	L_LINK(buffer_t) link;
	/*
	 * this will be the name that we show in tabs section, by default will be the filename
	 */
	char *name;
	char *filepath;
	/*
	 * total line count, initialized with count of opened file lines 
	 */
	uint64_t line_count;
	/*
	 * how many lines we passed from first line, starts of 1
	 */
	uint64_t line_offset;
	/*
	 * first line of buffer, we can access to other with line->link.next and ...
	 */
	line_t *first_line;
	/*
	 * keep tracking of current line, because in linked list it loose of speed to
	 * iterate over list every time that we want to acces it
	 */
	line_t *current_line;
	/*
	 * how many char we passed from beginning of string, starts with 0 (index)
	 */
	uint16_t char_offset;
	/*
	 * we may want to have some more buffer, we will store each buffer cursor pos
	 * here, so we can restore position
	 */
	cursor_pos_t pos;
	/*
	 * each buffer can be in one mode at a time, search, insert, lock or ...
	 */
	buffer_mode mode;
	/*
	 * set true if current buffer need to update, for example we don't want to render screen after moving cursor
	 * one line up or down
	 */
	bool render;
};

/*
 *	initialize new editor buffer, and set basic stuff of buffer
 *	if path is not NULL, it will open given path into buffer after
 *	initializing it
 */
public buffer_t *buffer_init(char *path);

/*
 * opens given file path into our gloabl editor variable 'editor'
 * also in currnet buffer, set set second arg to true for open in new buffer
 */
public return_message buffer_file_open(char *file_name);

/*
 * close current file of editor and opens new empty buffer
 */
public return_message buffer_file_close();

/*
 * save buffer into current open file in 'editor'
 */
public return_message buffer_file_save();

/*
 * load lines of given file into current buffer
 * also we can replace all current lines with file lines, or append
 * file to buffer
 */
public return_message buffer_file_load(char *filepath, line_load_mode mode);

/*
 * append given line next to current line
 * also if first_line is NULL so its gonna be appended as first line
 */
public return_message buffer_append_line(line_t *line);

/*
 * insert given key to current line
 */
public void buffer_insert_key(int key);

/*
 * return line by it's index, starts from 0
 * also we dont check if line index is greater to buffer line count :)
 */
public line_t *buffer_line_by_index(int index);

#endif
