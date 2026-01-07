#ifndef _BASIC_H
# define _BASIC_H

#include <stdbool.h>
#include <stdint.h>

#define EV_LOG

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

typedef struct cursor_pos cursor_pos_t;

struct cursor_pos {
	uint8_t row;
	uint8_t col;
};

#define SET_POS(pos, x, y) do {  \
	pos.row = x;                 \
	pos.col = y;                 \
} while(0);

#define MAKE_POS(x, y) (cursor_pos_t) { \
	.row = x, \
	.col = y  \
}

#define create_return_message(status, msg) ((return_message){status, msg})

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

typedef struct editor editor_t;
typedef struct buffer buffer_t;
typedef struct line line_t;
typedef struct promptbar promptbar_t;

void die(int code);

#endif
