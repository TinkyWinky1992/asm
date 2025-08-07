CC=gcc
CFLAGS=-Wall -Wextra -g
SRC=src/main.c src/utils.c src/parser/parser.c src/builder/builder.c src/store/store.c
OBJ=$(SRC:.c=.o)
TARGET=assmbler

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
