#ifndef _FILE_H
# define _FILE_H

#include "basic.h"

/*
 * returns pointer to start of file name in givn path
 * for example :
 * 		path : /src/main.c
 * points to here   ^ 
 */
public char *file_name(char *path);

public bool file_exists(char *path);

#endif
