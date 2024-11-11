## Sample Execution for Syntax Parser

**1. Heading with div tag, class and attributes**
```
##"This is Title!"
@div
["container-fluid dark"]{color:black height=90px}
```
input.txt
![image](https://github.com/user-attachments/assets/ca5872c6-c6cc-47c7-b24e-474d585158f5)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/7a11eae5-e5ba-4be9-957f-ddebbe6a1ee2)

parser_output.txt (AST shown with indentation)
![image](https://github.com/user-attachments/assets/006128b1-4eaa-4701-b876-e730b59bf578)

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
**Error Handling:** As the syntax parser expected a tag starting with an `@` symbol before the bracket `[` for class name, the parser generated a syntax error stating that `[` was unexpected.

**3. Missing Attributes inside `{` and `}`**
```
@div["container-fluid"]{} "Hello World"
```
input.txt
![image](https://github.com/user-attachments/assets/68518679-283e-4178-a40b-86750c7ce969)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/7c4bcc68-822b-49c0-b8ab-fd01d7322402)

parser_output.txt
![image](https://github.com/user-attachments/assets/f00e27ab-c7a4-46de-bc5c-23bd64f94fed)
**Error Handling:** The syntax parser expects text (attributes) inside `{` and `}`, and so it generates a syntax error stating that attributes are missing. Because it expects text after `{`, it does not expect a `}` to follow `{`, and hence throws a syntax error for the same.

**4. Generate Alert**
```
!!"generate alert"
```
input.txt
![image](https://github.com/user-attachments/assets/16d5bc34-98e5-422a-ba5e-24ecc2e95d10)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/16bbba18-0031-4c8e-bb3b-ce7b1fd44337)

parser_output.txt (AST shown with indentation)
![image](https://github.com/user-attachments/assets/1534718c-324b-4e4c-9f9d-cf3378a9c4cb)

**5. Using the Alert Operator `!!` without any Text following it
```
@div["container-fluid"]{width="300px"} !!
```
input.txt
![image](https://github.com/user-attachments/assets/822919bc-4670-406e-9b23-7ba01c8c671c)

output.txt (for lexer)
![image](https://github.com/user-attachments/assets/44a95430-4230-4a6f-8388-c9d1eb36bd5a)

parser_output.txt 
![image](https://github.com/user-attachments/assets/bdcb6d0d-fded-4b90-9f2a-afcab0b05b57)
**Error Handling:** As the syntax parser expects text after the `!!` alert operator, it throws a syntax error stating the same.




