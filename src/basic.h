#ifndef _BASIC_H
# define _BASIC_H

#include <stdbool.h>

#define public extern
#define private static

#define null NULL

typedef enum {
	ERROR,
	SUCCESS,
	TERMINATE
} operation_status;

typedef struct {
	operation_status status;
	char *message;
} return_message;

#define create_return_message(status, msg) ((return_message){status, msg})

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

#endif
