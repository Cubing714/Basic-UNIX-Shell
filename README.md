# Custom Shell

## Description
This is a lightweight custom shell implemented in C. It allows users to execute basic Linux commands, manage files, and perform simple operations directly from the terminal. The shell includes a few built-in commands for additional functionality.

## Features
- Execution of standard Linux commands
- Built-in commands:
  - `cd`: Change the current working directory
  - `pwd`: Display the current working directory
  - `ls`: List files in a directory
  - `echo`: Print arguments to the terminal
  - `find`: Search for files in a directory
  - `mkf`: Create a new file if it doesn’t exist
  - `mkdir`: Create a new directory if it doesn't exist
  - `clear`: Clear the terminal
  - `rm`: Remove a file, use -r to remove a directory
- Error handling for invalid commands or paths

## Requirements
- A C compiler (GCC recommended)
- A Linux-based operating system

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/Cubing714/Basic-UNIX-Shell.git
   cd Basic-UNIX-Shell

2. Build Project using make
  ```bash
   make

## Running Shell
1. Use build path in project directory
   ```bash
   cd bin
   ./shell
   
   ./bin/shell