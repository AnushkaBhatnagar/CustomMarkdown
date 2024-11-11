# CustomMarkdown

This is a compiler project to translate a Custom Markdown language to its HTML and Bootstrap CSS equivalent. The compiler has been designed in C++.

Project By: Anushka Bhatnagar (UNI: ab5920)

# Phase 1: Lexical Analyser
**Lexical Grammar Rules:**
1. Different levels of headings can be classified from h1-h4 using #, ##, ###, ####
2. Tag name begins with @ symbol.
3. Class name is enclosed within [ and ]
4. Attributes are enclosed within { and }
5. Text is enclosed within " and "
6. Different operators symbolize different meanings
   - `>` symboizes paragraph tag
   - `**` symbolizes bold
   - `*` symbolizes italics
   - `?` symbolizes form input
   - `!!` symbolizes alert

The Lexical Grammar for the language consists of the following token classes:

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
   ```
   cd CustomMarkdown
   ```
4. Install g++
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

An input.txt file is present with sample code. The lexical analyser can be run using the Shell Script as follows:

  ```
  ./run_lexer.sh input.txt
  ```

The output obtained from the lexical analyser gets stored in output.txt

## Error Handling and Recovery

The designed lexical analyser is using **Panic Mode** as a mechanism for error handling, wherein, if characters which are invalid in the language are detected, the lexer classifies them as "Unknown" and discards them until a token with a clear role is found.

Error Recovery entails skipping ahead to the next character and continuing parsing the input as per the rules of the designed language.

## Sample Execution

Sample inputs and outputs obtained from the lexical analyzer are described in [this link](https://github.com/AnushkaBhatnagar/CustomMarkdown/blob/main/Sample%20Inputs%20and%20Outputs/Lexer.md)

# Phase 2: Syntax Parser
**Context Free Grammar:**

- Document → ElementList EOF
- ElementList → Element ElementList | ε
- Element → Header | Tag | Alert
- Header → # Text | ## Text | ### Text | #### Text
- Tag → @ Text ClassSection AttributeSection | @ Text ClassSection | @ Text
- ClassSection → [ ClassName ]
- AttributeSection → { Attribute }
- Alert → !! Text
- Text → " TextString "

### Non-Terminals
- Document: The root of the syntax tree, representing the entire document.
- ElementList: A sequence of elements in the document.
- Element: An individual element such as a header, tag, or alert.
- Header: A heading element with different levels.
- Tag: A tagged element that may include class names and attributes.
- ClassSection: A class name enclosed in square brackets.
- AttributeSection: Attributes enclosed in curly braces.
- Alert: An alert element that starts with !!.
- Text: A block of text enclosed in double quotes.
  
### Terminals
- #, ##, ###, ####: Header symbols, indicating different levels of headings.
- @: Indicates the beginning of a tag.
- [, ]: Opening and closing brackets for class names.
- {, }: Opening and closing braces for attributes.
- !!: Indicates the beginning of an alert.
- ": Quotes used to enclose text.
- ClassName: A valid class name string.
- Attribute: A valid attribute string (e.g., color: black or height=10px).
- TextString: A valid text string enclosed in quotes.
- EOF: End-of-file marker.

## Running the Syntax Parser

The syntax parser takes its input from the output.txt file, which contains the output obtained from the lexer. Thus, after running the lexer as described in the steps above, the syntax parser can be run using the following Shell Script:

  ```
  ./parser
  ```

The output from the parser gets stored in parser_output.txt

## Error Handling

The parser handles errors through a dedicated mechanism that captures unexpected token sequences and reports syntax errors. When the parser encounters a token that does not conform to the expected syntax—such as missing elements or incorrectly structured tokens—it calls the syntaxError function, which logs an error message along with the position of the error in the input. This allows for easier debugging by indicating precisely where the parsing process failed. Additionally, the parser continues to process subsequent tokens after encountering an error, enabling it to identify multiple errors in a single parsing attempt.

## Sample Execution

Sample inputs and outputs obtained from the syntax parser are described in [this link](https://github.com/AnushkaBhatnagar/CustomMarkdown/blob/main/Sample%20Inputs%20and%20Outputs/Parser.md)
