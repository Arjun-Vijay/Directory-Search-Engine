.PHONY: clean build all

# compilation settings
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -std=gnu99 -Wstrict-prototypes -Wold-style-definition -g

# directory paths
INCLUDE_DIR = ./
SRC_DIR = ./
OBJ_DIR = ./

# file lists
CFILES = hashmap.c test.c llist.c
OBJS = hashmap.o test.o llist.o

# binary
BIN = test

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) -lm

clean:
	rm -f $(OBJS) $(BIN) *~
