#ifndef _EDITOR_H
# define _EDITOR_H

#include <stdint.h>
#include "basic.h"
#include "buffer.h"
#include "line.h"
#include "../llink/llist.h"

#define TAB_SIZE 4


struct editor {
	// fd for input and output ttys
	int tty_in;
	int tty_out;

	buffer_t *current_buffer;

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
public void editor_render_line(line_t *line);
/*
 * checks if cursor is out of the current line, if its is, will move cursor to end of current line
 */
public void editor_check_offset();

/*
 * this is just a helper method to get current buffer of editor
 */
public buffer_t *editor_buffer();

#endif
