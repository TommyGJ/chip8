CC = gcc

CFLAGS = -g -Wall 

DEPS = chip.h cpu_helpers.h opcodes.h

OBJ = opcodes.o cpu_helpers.o chip.o main.o

LIBS = -Iinclude -lpthread -Llib -lSDL2 -lSDL2main -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chip8: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 
	$(RM) *.o *~

