#ifndef _STATUS_BAR_H
# define _STATUS_BAR_H

#include "basic.h"
#include "line.h"

struct statusbar {
	/*
	 * margin from top of screen, usually equals to editor.rows - 1
	 */
	int margin;
	int height;
	
	line_t content;
};

#endif
