# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I./include   # Include the header files directory
LDFLAGS =                       # Linker flags (if any)

# Create the bin directory if it doesn't exist
$(BIN_DIR) $(OBJ_DIR) $(INCLUDE_DIR) $(HOME_DIR) $(CONFIG_DIR):
	mkdir -p $@

# Directories
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
INCLUDE_DIR = ./include
HOME_DIR = ./home              # User home directory
CONFIG_DIR = ./config          # Configuration directory

# Source files (find .c files in specified directories)
SRC = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Final executable
EXEC = $(BIN_DIR)/shell        

# Default target (make all)
all: $(BIN_DIR) $(EXEC)

# Rule to link object files into the final executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Rule to compile each .c file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target (not a file)
.PHONY: all clean