#ifndef _EDITOR_H
# define _EDITOR_H

#include <stdint.h>
#include "basic.h"
#include "../llink/llist.h"

typedef struct cursor_pos cursor_pos_t;

typedef struct editor editor_t;
typedef struct editor_buffer editor_buffer_t;
typedef struct editor_buffer_line editor_buffer_line_t;

typedef enum {
	MODE_INSERT,
	MODE_LOCK,
	MODE_PROMPT
} editor_mode;

struct cursor_pos{
	uint8_t row;
	uint8_t col;
};

struct editor_buffer_line {
	L_LINK(editor_buffer_t) link;
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
	 * we may want to have some more buffer, we will store each buffer cursor pos
	 * here, so we can restore position
	 */
	cursor_pos_t pos;
};

struct editor {
	// fd for input and output ttys
	int tty_in;
	int tty_out;

	char *file_path;
	editor_mode mode;

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
