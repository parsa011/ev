CC = gcc
PROGRAM = ev
BUILD_PATH = build
CC_FLAGS = -g -Wall

SRCS = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)

.PHONY : main
main : $(SRCS) $(HEADERS)
	$(CC) $(CC_FLAGS) $^ -o $(BUILD_PATH)/$(PROGRAM)
