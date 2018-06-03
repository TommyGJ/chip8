CC = gcc

CFLAGS = -g -Wall 

DEPS = chip.h cpu_helpers.h

OBJ = opcodes.o cpu_helpers.o

LIBS = -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

opcodes: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 
	$(RM) *.o *~
