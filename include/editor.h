#ifndef _EDITOR_H
# define _EDITOR_H

#include <stdint.h>
#include "basic.h"
#include "buffer.h"
#include "line.h"
#include "../llink/llist.h"
#include "statusbar.h"
#include "prompt.h"

#define TAB_SIZE 4

struct editor {
	// fd for input and output ttys
	int tty_in;
	int tty_out;

	buffer_t *current_buffer;
	struct statusbar_t {
		/*
		 * margin from top of screen, usually equals to editor.rows - 1
		 */
		int margin;
		int height;

		line_t content;
	} statusbar;

	int rows;
	int cols;
};

#define BUFFER_ROW editor.rows - 1

/*
 *	time to second that we want to show messages to user
 */
#define USER_MESSAGE_TIME 2

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
 * append given buffer next to the current buffer
 */
public void editor_buffer_append(buffer_t *buf);

/*
 * means go to the given buffer and set it as current buffer
 */
public void editor_buffer_change(buffer_t *buf);

/*
 * runs editor infinite loop, taking command and doing them
 */
public return_message editor_run();

/*
 * writes current buffer into screen
 */
public return_message editor_render();

/*
 * writes current buffer into screen
 */
public void editor_render_buffer();

/*
 * writes information of current buffer into status bar
 */
public void editor_render_statusbar();

/*
 * writes single file into buffer , at current position of screen (whereever that cursor is)
 * also will do line render stuff, like showing tabs by specified tab size and ...
 */
public void editor_render_line(line_t *line);

/*
 * this is just a helper method to get current buffer of editor
 */
public buffer_t *editor_buffer();

#endif
