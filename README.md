# CustomMarkdown

This is a compiler project to translate a Custom Markdown language to its HTML and Bootstrap CSS equivalent. The compiler has been designed in C++.

## Lexical Analyser

| Token Class | Description | Example |
|  :---:        |     :---:     |        :---: |
| Tag   | Defined by `@` followed by Tag Name    | @div  |
| Header   | #, ##, ### for different levels of headings    | #Heading  |
| Text    | Any plain text between double quotes `"` and `"`| "Example"|
| ClassName  | Text between `[` and `]`    | [container-fluid]   |
| Attribute   | Key-Value pairs between `{` and `}`    | {width="300px"}   |
| Operators  | Symbols including **, *, >, !!, ?, =, : <br> Each symbol corresponds to a different meaning | !!Alert   |
| LBracket  | Left Bracket for Class Names    | `[`   |
| RBracket  | Right Bracket for Class Names    | `]`   |
| LBrace | Left Brace for Attributes    | `{`   |
| RBrace  |  Right Brace for Attributes    | `}`   |

## Set Up

1. Clone the Repository
   ```
   git clone https://github.com/AnushkaBhatnagar/CustomMarkdown
   ```
2. Open the project folder
3. Install g++
   - On Ubuntu/Debian
     ```
     sudo apt update
     sudo apt install g++
     ```
   - On macOS
     ```
     xcode-select --install
     ```
5. Make the Shell Script Executable
   ```
   chmod +x run_lexer.sh
   ```

## Running the Lexical Analyser

- Using the Shell Script
  ```
  ./run_lexer.sh
  ```
  **OR**
- Manual Compilation and Execution
  ```
  g++ lexer.cpp -o lexer_program
  ./lexer_program
  ```
