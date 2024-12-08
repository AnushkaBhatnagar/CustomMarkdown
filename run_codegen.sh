#!/bin/bash

# Check if the output from the parser exists
if [ ! -f "parser_output.txt" ]; then
    echo "Error: File 'parser_output.txt' not found! Make sure to run the parser first to generate this file."
    exit 1
fi

# Compile the C++ code for code generation
echo "Compiling codegen.cpp..."
g++ codegen.cpp -o codegen

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful. Running the code generation..."

# Run the code generation
./codegen

# Check if the code generation ran successfully
if [ $? -eq 0 ]; then
    echo "Code generation executed successfully. Check 'output.html' for the generated HTML."
else
    echo "Code generation execution failed."
    exit 1
fi
