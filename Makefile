
CC = gcc

CFLAGS = -Wall -Wextra

LDFLAGS = -lm

TARGET = programa

SRC = main.c

OBJ = main.o a.out

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
