## Sample Execution for Lexical Analyser

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