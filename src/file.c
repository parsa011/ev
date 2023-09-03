#include <stddef.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include "file.h"

public char* file_name(char* path)
{
	size_t i = 0;
	size_t pos = 0;
	char* p = (char*)path;
	while (*p) {
		i++;
		if (*p == '/' || *p == '\\') {
			pos = i;
		}
		p++;
	}
	return path + pos;
}

public bool file_exists(char *path)
{
	assert(path);
	return access(path, F_OK) == 0;
}
