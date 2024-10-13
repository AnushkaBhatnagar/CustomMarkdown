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
| Operators  | Symbols including **, *, >, !!, ?, -, =, : <br> Each symbol corresponds to a different meaning | !!Alert   |
| Lbracket  | Left Bracket for Class Names    | `[`   |
| Rbracket  | Right Bracket for Class Names    | `]`   |
| LBrace | Left Brace for Attributes    | `{`   |
| RBrace  |  Right Brace for Attributes    | `}`   |

## Installation and Set Up

1. Clone the Repository
   ```
   git clone https://github.com/AnushkaBhatnagar/CustomMarkdown
   ```
2. Navigate to the project folder
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
## Error Handling and Recovery

The designed lexical analyser is using **Panic Mode** as a mechanism for error handling, wherein, if characters which are invalid in the language are detected, the lexer classifies them as "Unknown" and discards them until a token with a clear role is found.

Error Recovery entails skipping ahead to the next character and continuing parsing the input as per the rules of the designed language.

## Sample Execution

**1. Creating a div** <br>
   **Input**
   ```
   @div["container-fluid"]{width="300px"} "Hello World"
   ```
   **Output**
   ```
   <Tag, div>
   <Lbracket, [>
   <ClassName, container-fluid>
   <Rbracket, ]>
   <LeftBrace, {>
   <Attribute, width=300px>
   <RightBrace, }>
   <Text, Hello World>
   <EndOfFile, >
   ```

**2. Creating a nav (with invalid tokens)** <br>
   **Input**
   ```
   @nav["top"]{height="50px"} xyz &"Site Title",;
   ```
   **Output**
   ```
   <Tag, nav>
   <Lbracket, [>
   <ClassName, top>
   <Rbracket, ]>
   <LeftBrace, {>
   <Attribute, height=50px>
   <RightBrace, }>
   Unknown character: x
   Unknown character: y
   Unknown character: z
   Unknown character: &
   <Text, Site Title>
   Unknown character: ,
   Unknown character: ;
   <EndOfFile, >
   ```
   **Error Handling:** Characters not defined by the language are classified as unknown, and the lexer continues parsing the input to detect tokens with valid roles. <br> <br>
**3. Creating a heading and div (with invalid characters and incomplete syntax)** <br>
   **Input**
   ```
   #"Title" 123 @div[;
   ```
   **Output**
   ```
   <Header, #>
   <Text, Title>
   Unknown character: 1
   Unknown character: 2
   Unknown character: 3
   <Tag, div>
   <Lbracket, [>
   Unknown character: ;
   <EndOfFile, >
   ```
   **Error Handling:** Characters not defined by the language are classified as unknown, and the lexer continues parsing the input to detect tokens with valid roles. The lexer does not throw an error for incomplete syntax for div, as that would occur in the Syntax Analysis phase. <br> <br>
**4. Creating an attribute list** <br>
   **Input**
   ```
   {width=50px height=10px}
   ```
   **Output**
   ```
   <LeftBrace, {>
   <Attribute, width=50px height=10px>
   <RightBrace, }>
   <EndOfFile, >
   ```
   **No Error Handling** as the given tokens are valid lexemes. The incomplete syntax will be parsed in the next (syntax) phase of compiler design. <br> <br>
**5. Sample Text** <br>
   **Input**
   ```
   "Sample Text" "a
   ```
   **Output**
   ```
   <Text, Sample Text>
   Unknown character: "
   Unknown character: a
   <EndOfFile, >
   ```
   **Error Handling:** The string in double quotes gets classified as Text, but the remainder of the input is classified as Unknown because of the missing quote at the end. 
