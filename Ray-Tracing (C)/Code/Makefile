TARGET ?= exec
CC = gcc
SOURCES_DIR = src
OBJECTS_DIR = obj
CFLAGS ?= -Wall -g
OBJS = $(shell find .)
OBJS := $(shell find $(SOURCES_DIR) -name "*.c" |sed 's/.c$$/.o/g' |sed 's/$(SOURCES_DIR)/$(OBJECTS_DIR)/g')
ifeq ($(shell uname), Linux)
	LLIB ?= -L/usr/lib
	LDIR ?= -lfreeimage -lm
	IDIR ?= -I/usr/include
else
	LLIB ?= -L/opt/homebrew/lib
	LDIR ?= -lfreeimage -lm
	IDIR ?= -I/opt/homebrew/include
endif

LLIB ?= -L/opt/homebrew/lib
LDIR ?= -lfreeimage -lm
IDIR ?= -I/opt/homebrew/include

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LLIB) $(LDIR) $(IDIR)

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(IDIR)

clean:
	rm -rf $(OBJECTS_DIR)/*
	rm -f exec