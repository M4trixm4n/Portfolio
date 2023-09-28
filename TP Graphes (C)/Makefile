CC = gcc
CFLAGS ?= -Wall -g
LDFLAGS ?= -g
TARGET ?= reseau
SRC := $(wildcard ./*.c)
OBJS := $(SRC:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

./%.o: ./%.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ./*.o 

mrproper: clean
	rm -f $(TARGET)