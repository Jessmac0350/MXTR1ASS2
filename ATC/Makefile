CC=gcc
CFLAGS=-g -Wall -Wextra -Iinclude
LIBS=-lm
TARGET=atc

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
# you may need to put your extra files here
_DEPS = message.h aircraft.h requests.h calculations.h
_OBJS = main.o aircraft.o requests.o calculations.o

DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) 
