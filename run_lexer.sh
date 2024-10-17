#!/bin/bash

# Check if the user has provided a filename as an argument
if [ $# -ne 1 ]; then
    echo "Usage: ./run_lexer.sh <input_file>"
    exit 1
fi

INPUT_FILE=$1

# Check if the input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: File '$INPUT_FILE' not found!"
    exit 1
fi

# Compile the C++ code
echo "Compiling lexer.cpp..."
g++ lexer.cpp -o lexer

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful. Running the lexer..."

# Run the lexer with the provided input file
./lexer "$INPUT_FILE"
