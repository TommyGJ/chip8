CC = gcc

CFLAGS = -g -Wall 

SRC_DIR = src

OBJ_DIR = obj

DEPS := $(wildcard $(SRC_DIR)/*.h)

SRC := $(wildcard $(SRC_DIR)/*.c)

OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBS = -Iinclude -lpthread -Llib -lSDL2 -lSDL2main -lm

chip8: $(OBJ)
	@$(CC) $(OBJ) $(LIBS) -o $@

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean: 
	$(RM) $(OBJ)

