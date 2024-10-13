#!/bin/bash

# Check if g++ is installed
if ! command -v g++ &> /dev/null
then
    echo "g++ could not be found. Please install g++."
    exit
fi

# Compile the lexer.cpp file
echo "Compiling lexer.cpp..."
g++ lexer.cpp -o lexer_program

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful."
    # Run the lexer program
    echo "Executing lexer_program..."
    ./lexer_program
else
    echo "Compilation failed. Please check the source code for errors."
fi
