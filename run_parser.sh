
#!/bin/bash

# Check if the output from the lexer exists
if [ ! -f "output.txt" ]; then
    echo "Error: File 'output.txt' not found! Make sure to run the lexer first to generate this file."
    exit 1
fi

# Compile the C++ code for the parser
echo "Compiling parser.cpp..."
g++ parser.cpp -o parser

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful. Running the parser..."

# Run the parser
./parser

# Check if the parser ran successfully
if [ $? -eq 0 ]; then
    echo "Parser executed successfully. Check 'parser_output.txt' for the generated AST."
else
    echo "Parser execution failed."
    exit 1
fi