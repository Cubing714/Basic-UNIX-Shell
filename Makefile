# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I./include   # Include the header files directory
LDFLAGS =                       # Linker flags (if any)

# Directories
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

# Files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/shell.c $(SRC_DIR)/commands.c $(SRC_DIR)/utils.c    # Add your .c files here
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/shell.o $(OBJ_DIR)/commands.o $(OBJ_DIR)/utils.o   # Corresponding .o files
EXEC = $(BIN_DIR)/shell        # Final executable

# Default target (make all)
all: $(BIN_DIR) $(EXEC)

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rule to link object files into the final executable
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# Rule to compile each .c file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target (not a file)
.PHONY: all clean