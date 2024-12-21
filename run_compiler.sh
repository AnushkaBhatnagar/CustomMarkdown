#!/bin/bash

# Log file to record errors and status
LOG_FILE="compiler_log.txt"
> $LOG_FILE  # Clear the log file at the start

# Function to log messages
log_message() {
    echo "$1"
    echo "$(date): $1" >> $LOG_FILE
}

# Lexing Phase
log_message "Starting Lexing Phase..."
if [ ! -s "input.txt" ]; then  # Check if input.txt exists and is not empty
    log_message "Error: input.txt not found or empty. Exiting."
    exit 1
fi

# Run lexer
g++ lexer.cpp -o lexer
if [ $? -ne 0 ]; then
    log_message "Error: Lexer compilation failed."
    #exit 1
fi

./lexer input.txt > output.txt
LEXER_STATUS=$?  # Capture the exit status of lexer
if [ $LEXER_STATUS -ne 0 ]; then # if status != 0
    log_message "Error: Lexing failed." 
    # Skipping Parsing and Code Generation.
    #exit 1
else # if lexer status = 0
    log_message "Lexing completed successfully. Output written to output.txt."
    
    # Parsing Phase
    log_message "Starting Parsing Phase..."
    if [ ! -s "output.txt" ]; then  # Check if output.txt exists and is not empty
        log_message "Error: output.txt not found or empty. Exiting."
        #exit 1
    fi

    # Run parser
    g++ parser.cpp -o parser
    if [ $? -ne 0 ]; then 
        log_message "Error: Parser compilation failed."
        #exit 1
    fi

    ./parser output.txt > parser_output.txt
    PARSER_STATUS=$? # Capture the exit status of parser
    if [ $PARSER_STATUS -ne 0 ]; then # if status != 0
        log_message "Error: Parsing failed. Please check the parser."
        #exit 1
    else # if parser status = 0
        log_message "Parsing completed successfully. Output written to parser_output.txt."
        
        # Code Generation Phase
        log_message "Starting Code Generation Phase..."
        if [ ! -s "parser_output.txt" ]; then  # Check if parser_output.txt exists and is not empty
            log_message "Error: parser_output.txt not found or empty. Exiting."
            exit 1
        fi

        # Run codegen
        g++ codegen.cpp -o codegen
        if [ $? -ne 0 ]; then
            log_message "Error: Code generation compilation failed."
            #exit 1
        fi

        ./codegen > output.html
        if [ $? -ne 0 ]; then
            log_message "Error: Code generation failed. Please check the codegen."
            #exit 1
        else
            log_message "Code generation completed successfully. Output written to output.html."
        fi

        log_message "Compiler run completed successfully."
    fi
fi
