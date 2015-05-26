CC = gcc
CFLAGS = 

SOURCES = decodeStage.c dump.c executeStage.c fetchStage.c main.c \
					memory.c memoryStage.c registers.c tools.c writebackStage.c
OBJECTS = $(SOURCES:%.c=%.o)
BIN = lab3

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BIN)
	$(RM) *.o
