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
	 * how many lines we cant write into the screen ?
	 */
	uint16_t rows;
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
	/*
	 * did we change buffer ? yes if there was any change
	 */
	bool dirty;
};

/*
 *	initialize new editor buffer, and set basic stuff of buffer
 *	if path is not NULL, it will open given path into buffer after
 *	initializing it
 */
public buffer_t *buffer_init(char *path, int rows);

/*
 * call it after any change to buffer texts
 */
public void buffer_dirty();

/*
 * opens given file path into our gloabl editor variable 'editor'
 * also in currnet buffer, set set second arg to true for open in new buffer
 */
public return_message buffer_file_open(char *file_name);

/*
 * just a helper method to set filename and path for given buffer
 * we got buffer as an arg, because we may want to set name for new alloced buffer
 */
public void buffer_set_file_name(buffer_t *buf, char *filepath);

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
 * checks if cursor is out of the current line, if its is, will move cursor to end of current line
 */
public void buffer_check_offset();

/*
 * set buffer char offset to given offset and update cursor col
 */
public void buffer_go_to_offset(int offset);

/*
 * go to line, index based
 */
public void buffer_go_to_line(int index);

/*
 * return current char under the cursor
 */
public char *buffer_current_char();

/*
 * return nth char of current line, with given offset, also program will panic when offset is not valid
 * means too large, or negative
 */
public char *buffer_char_at(int offset);

/*
 * return line by it's index, starts from 0
 * also we dont check if line index is greater to buffer line count :)
 */
public line_t *buffer_line_by_index(int index);

/*
 * delete current line fully and go to next or prev line
 */
public void buffer_delete_line(bool go_next);

/*
 * open new line in down of current line
 */
public void buffer_open_line();

/*
 * make given buffer memory free , also with its lines
 */
public void buffer_free(buffer_t *buf);

#endif
