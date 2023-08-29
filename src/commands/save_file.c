#include <stdio.h>
#include "commands/save_file.h"
#include "editor.h"
#include "buffer.h"

public return_message save_file_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (!buf->filepath) {
		// PROMPT and get file name
		return create_return_message(ERROR, "TODO : implement get buffer name");
	}
	FILE *fp = fopen(buf->filepath, "w+");
	if (!fp) {
		die(0);
	}
	line_t *ln = buf->first_line;
	for (int i = 0; i < buf->line_count; i++) {
		fprintf(fp, "%s\n", ln->str);
		ln = L_LINK_NEXT(ln);
	}
	fclose(fp);
	buf->dirty = false;
	buf->render = true;
	return create_return_message(SUCCESS, "file saved");
}
