#ifndef _EDITOR_H
# define _EDITOR_H

#include <stdint.h>
#include "basic.h"
#include "../llink/llist.h"

#define TAB_SIZE 4

typedef struct editor editor_t;
typedef struct editor_buffer editor_buffer_t;
typedef struct editor_buffer_line editor_buffer_line_t;

typedef enum {
	MODE_INSERT,
	MODE_LOCK,
	MODE_PROMPT
} editor_buffer_mode;

typedef enum {
	APPEND,
	REPLACE
} line_load_mode;

struct editor_buffer_line {
	L_LINK(editor_buffer_line_t) link;

	char *str;
	int len;
};

struct editor_buffer {
	L_LINK(editor_buffer_t) link;
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
	editor_buffer_line_t *first_line;
	/*
	 * keep tracking of current line, because in linked list it loose of speed to
	 * iterate over list every time that we want to acces it
	 */
	editor_buffer_line_t *current_line;
	/*
	 * how many char we passed from beginning of string, starts with 0 (index)
	 */
	uint8_t char_offset;
	/*
	 * we may want to have some more buffer, we will store each buffer cursor pos
	 * here, so we can restore position
	 */
	cursor_pos_t pos;
	/*
	 * each buffer can be in one mode at a time, search, insert, lock or ...
	 */
	editor_buffer_mode mode;
	/*
	 * set true if current buffer need to update, for example we don't want to render screen after moving cursor
	 * one line up or down
	 */
	bool render;
};

struct editor {
	// fd for input and output ttys
	int tty_in;
	int tty_out;

	editor_buffer_t *current_buffer;

	int rows;
	int cols;
};

public editor_t editor;

/*
 * init editor in raw mode and other startup configurations
 */
public void editor_init();

/*
 * gets size of terminal, also used everytime that we want to handle
 * window size changed signal
 */
public void editor_change_size();

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
 * writes current buffer into screen
 */
public return_message editor_render();

/*
 * writes single file into buffer , at current position of screen (whereever that cursor is)
 * also will do line render stuff, like showing tabs by specified tab size and ...
 */
public void editor_render_line(editor_buffer_line_t *line);

/*
 * return line by it's index, starts from 0
 * also we dont check if line index is greater to buffer line count :)
 */
public editor_buffer_line_t *editor_buffer_line_by_index(int index);

/*
 * checks if cursor is out of the current line, if its is, will move cursor to end of current line
 */
public void editor_check_offset();

/*
 * this is just a helper method to get current buffer of editor
 */
public editor_buffer_t *editor_buffer();

/*
 *	initialize new editor buffer, and set basic stuff of buffer
 *	if path is not NULL, it will open given path into buffer after
 *	initializing it
 */
public editor_buffer_t *editor_buffer_init(char *path);

/*
 * opens given file path into our gloabl editor variable 'editor'
 * also in currnet buffer, set set second arg to true for open in new buffer
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

/*
 * load lines of given file into current buffer
 * also we can replace all current lines with file lines, or append
 * file to buffer
 */
public return_message editor_file_load_lines(char *filepath, line_load_mode mode);

/*
 * initialie new buffer line and copy given string to line str
 */
public editor_buffer_line_t *editor_buffer_line_init(char *str, int len);

/*
 * append given line next to current line
 * also if first_line is NULL so its gonna be appended as first line
 */
public return_message editor_buffer_line_append(editor_buffer_line_t *line);

#endif
