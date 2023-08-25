CC = gcc
PROGRAM = ev
BUILD_PATH = build
CC_FLAGS = -g -Wall
INCLUDES = -I./include

SRCS = $(wildcard src/*.c src/commands/*.c)
HEADERS = $(wildcard src/*.h src/commands/*.h)

.PHONY : main
main : $(SRCS) $(HEADERS)
	$(CC) $(INCLUDES) $(CC_FLAGS) $^ -o $(BUILD_PATH)/$(PROGRAM)

clear:
	rm -rf ./$(BUILD_PATH)/*
