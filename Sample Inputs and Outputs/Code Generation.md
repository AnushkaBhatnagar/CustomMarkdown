## Sample Execution for the Final Compiler (Code Generation)

**1. Heading with div tag, class and attributes**
```
##"This is Title!"
@div
["container-fluid dark"]{color:black height=90px}
```
terminal
![image](https://github.com/user-attachments/assets/703934b4-2a6a-4071-8719-ca951fb112e3)

input.txt
![image](https://github.com/user-attachments/assets/ca5872c6-c6cc-47c7-b24e-474d585158f5)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/7a11eae5-e5ba-4be9-957f-ddebbe6a1ee2)

parser_output.txt (AST shown with indentation)
![image](https://github.com/user-attachments/assets/006128b1-4eaa-4701-b876-e730b59bf578)

output.html
![image](https://github.com/user-attachments/assets/d96f1b38-3df2-45bf-a4ef-6f4fb08c6f56)

Running on Server
![image](https://github.com/user-attachments/assets/cdf698f2-34a4-49f2-be99-03142791431f)

**2. Missing Tag**
```
["container-fluid"]{width="300px"} "Hello World"
```
input.txt
![image](https://github.com/user-attachments/assets/5a05e091-b55c-435c-9947-039ec5b6a4b0)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/a3bc1403-3b24-4721-a359-eb03797b1db3)

parser_output.txt
![image](https://github.com/user-attachments/assets/be11a2d9-9721-48ad-9f98-4d0ac6bdf04d)

terminal
![image](https://github.com/user-attachments/assets/256eb2cf-4d87-430b-bced-a703462d60d1)

**Error Handling:** As an error is detected in the parsing phase (no html tag has been defined), the compiler does not execute the code generation phase and hence no output code is generated in output.html.

**3. Unknown Symbol `^`**
```
@div["container-fluid"]^
```
input.txt
![image](https://github.com/user-attachments/assets/8f510859-d4ee-446f-8702-c1457658751e)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/fd9c334b-b998-4b7d-9715-66a9c587f5e1)

terminal
![image](https://github.com/user-attachments/assets/ec4e13d7-1ec1-4e21-b040-ae5792d124dc)

**Error Handling:** As an error is encountered at the lexical phase, the compiler does not execute the remaining phases and logs the respective error.

**4. Heading, Classes, Attribute, Text**
```
#"Anushka Bhatnagar"@div["container-fluid bg-dark text-light"]{height=200px}"Columbia University"
```
terminal
![image](https://github.com/user-attachments/assets/0eb4a859-67a3-4afb-825a-dc2b8f8944a6)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/1567660b-48b6-42e4-ae5a-037a8085b166)

output.html
![image](https://github.com/user-attachments/assets/33e6f4b4-fe39-4916-bed9-714d3566f104)

Running on Server
![image](https://github.com/user-attachments/assets/9e832829-98d4-4026-88f6-a42e92eacde7)

**5. Using the Alert Operator `!!` without any Text following it**
```
@div["container-fluid"]{width="300px"} !!
```
input.txt
![image](https://github.com/user-attachments/assets/822919bc-4670-406e-9b23-7ba01c8c671c)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/44a95430-4230-4a6f-8388-c9d1eb36bd5a)

parser_output.txt 
![image](https://github.com/user-attachments/assets/bdcb6d0d-fded-4b90-9f2a-afcab0b05b57)

terminal
![image](https://github.com/user-attachments/assets/ca17ddd3-3c41-45d8-994f-f4c319cde1e8)

**Error Handling:** As the syntax parser expects text after the `!!` alert operator, it throws a syntax error stating the same. Because of an error at this stage, the compiler does not execute the code generation phase and logs the respective error.
