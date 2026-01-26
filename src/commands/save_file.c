#include <stdio.h>
#include <stdlib.h>
#include "commands/save_file.h"
#include "editor.h"
#include "buffer.h"
#include "prompt.h"

return_message_t save_file_command(char **args)
{
	buffer_t *buf = editor_buffer();
	if (!buf->filepath) {
		char *file_name = prompt_string("file to save in : ");
		if (!file_name) {
			return CREATE_RETURN_MESSAGE(ERROR, "canceled");
		}
		buffer_set_file_name(buf, file_name);
		free(file_name);
	}
	// TODO : write into file should be an external api
	FILE *fp = fopen(buf->filepath, "w+");
	if (!fp) {
		die(0);
	}
	line_t *ln = buf->first_line;
	for (int i = 0; i < buf->line_count; i++) {
		for (int i = 0; i < ln->len; i++) {
			fputc(*(ln->str + i), fp);
		}
		fputc('\n', fp);
		ln = L_LINK_NEXT(ln);
	}
	fclose(fp);
	buf->dirty = false;
	buf->render = true;
	return CREATE_RETURN_MESSAGE(SUCCESS, "file saved");
}
