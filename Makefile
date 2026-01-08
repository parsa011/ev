CC = gcc
PROGRAM = ev
BUILD_PATH = build
CC_FLAGS = -g -Wall
INCLUDES = -I./include

SRCS = $(wildcard src/*.c src/commands/*.c)
HEADERS = $(wildcard src/*.h src/commands/*.h)

main : $(SRCS) $(HEADERS)
	$(CC) $(INCLUDES) $(CC_FLAGS) $^ -o $(BUILD_PATH)/$(PROGRAM) -lm

run : main
	$(BUILD_PATH)/$(PROGRAM)

clear:
	rm -rf ./$(BUILD_PATH)/*

.PHONY : main
.PHONY : run
