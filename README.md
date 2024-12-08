# CustomMarkdown

This is a compiler project to translate a Custom Markdown language to its HTML and Bootstrap CSS (v5.3.3) equivalent. The compiler has been designed in C++.

Different Phases Implemented:
- [Lexical Analyser](#phase-1---lexical-analyser)
- [Syntax Parser](#phase-2---syntax-parser)
- [Code Generation](#phase-3---code-generation)

Project By: Anushka Bhatnagar (UNI: ab5920)

# Execution

The below steps can be followed to run all phases together:

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

## Running the Compiler

An input.txt file is present with sample code. The compiler can be run using the Shell Script as follows:

  ```
  ./run_compiler.sh
  ```

The output obtained from the compiler gets stored in output.html

# Sample Inputs and Outputs

Sample inputs and outputs obtained from the compiler are described in [this link](https://github.com/AnushkaBhatnagar/CustomMarkdown/blob/main/Sample%20Inputs%20and%20Outputs/Code%20Generation.md)

# Detailed Description of Different Phases

The description below explains each phase of this compiler in detail. Though the compiler in its entirety can be run using the code above, code for running each phase individually is also present below.

# Phase 1 - Lexical Analyser
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

# Phase 2 - Syntax Parser
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

The syntax parser takes its input from the output.txt file, which contains the output obtained from the lexer. If running individually, after running the lexer as described in the steps above, the syntax parser can be run using the following Shell Script:

  ```
  ./run_parser.sh
  ```

The output from the parser gets stored in parser_output.txt

## Error Handling

The parser handles errors through a dedicated mechanism that captures unexpected token sequences and reports syntax errors. When the parser encounters a token that does not conform to the expected syntax—such as missing elements or incorrectly structured tokens—it calls the syntaxError function, which logs an error message along with the position of the error in the input. This allows for easier debugging by indicating precisely where the parsing process failed. Additionally, the parser continues to process subsequent tokens after encountering an error, enabling it to identify multiple errors in a single parsing attempt.

## Sample Execution

Sample inputs and outputs obtained from the syntax parser are described in [this link](https://github.com/AnushkaBhatnagar/CustomMarkdown/blob/main/Sample%20Inputs%20and%20Outputs/Parser.md)

The demo video demonstraring the working of the syntax parser can be found [here](https://drive.google.com/file/d/1WPGqp0m2cf6LVfs_6w3EoB3QuYQ8kHGk/view?usp=sharing) (can be accessed through Columbia Mail ID)

# Phase 3 - Code Generation

The Code Generation Phase takes the AST generated from the Parsing Phase as input, and generates its corresponding HTML and Bootstrap CSS equivalent in the output.html file as the final code.

## Running Code Generation

If running individually, this phase can be executed after syntax analyis as described in the steps above. This phase can be run using the following Shell Script:

  ```
  ./run_codegen.sh
  ```
The output specific to the code generation phase (such as displaying the nodes created after parsing the AST obtained from the step above) are logged in log.txt.
The final HTML and Bootstrap CSS code generated gets stored in output.html.

## Error Handling

The code generation phase checks for any errors during compilation and displays the same. This phase does not execute in case there were errors detected in either of the two earlier phases. The respective error gets logged and the corresponding stage is indicated in display along with the error encountered. 

## Sample Execution

Sample inputs and outputs obtained from the code generation phase (the final compiler) are described in [this link](https://github.com/AnushkaBhatnagar/CustomMarkdown/blob/main/Sample%20Inputs%20and%20Outputs/Code%20Generation.md)

The demo video demonstraring the working of the final compiler can be found [here](https://drive.google.com/file/d/1Z7VMz0Kn-tbQEh6Y0OAagGR9y3yvzoTJ/view?usp=sharing) (can be accessed through Columbia Mail ID)
