#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
int x=0;
// Token types based on your grammar
enum class TokenType {
    Operator, Tag, ClassName, Attribute, Lbracket, Rbracket,
    LeftBrace, RightBrace, Text, Header, LexerError, EndOfFile
};

ostream& operator<<(ostream& os, const TokenType& type) {
    switch (type) {
        case TokenType::Operator: return os << "Operator";
        case TokenType::Tag: return os << "Tag";
        case TokenType::ClassName: return os << "ClassName";
        case TokenType::Attribute: return os << "Attribute";
        case TokenType::Lbracket: return os << "Lbracket";
        case TokenType::Rbracket: return os << "Rbracket";
        case TokenType::LeftBrace: return os << "LeftBrace";
        case TokenType::RightBrace: return os << "RightBrace";
        case TokenType::Text: return os << "Text";
        case TokenType::Header: return os << "Header";
        case TokenType::LexerError: return os << "LexerError";
        case TokenType::EndOfFile: return os << "EndOfFile";
        default: return os << "Unknown";
    }
}

struct Token {
    TokenType type;
    string value;
};

// AST Node structure
struct ASTNode {
    string type;
    string value;
    vector<ASTNode*> children;

    ASTNode(string t, string v) : type(t), value(v) {}
    ~ASTNode() {
        for (auto child : children) {
            delete child;
        }
    }
};

// Helper function to read tokens from output.txt
vector<Token> readTokens(const string &filename) {
    vector<Token> tokens;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream stream(line);
        string type, value;
        stream >> type;

        // Read the rest of the line as value
        getline(stream, value);
        
        type = type.substr(1, type.size() - 2); // Remove angle brackets from the type
        value = value.substr(1, value.size() - 2); // Remove quotes from the value

        // Map string to TokenType
        TokenType tokenType;
        if (type == "Operator") tokenType = TokenType::Operator;
        else if (type == "Tag") tokenType = TokenType::Tag;
        else if (type == "ClassName") tokenType = TokenType::ClassName;
        else if (type == "Attribute") tokenType = TokenType::Attribute;
        else if (type == "Lbracket") tokenType = TokenType::Lbracket;
        else if (type == "Rbracket") tokenType = TokenType::Rbracket;
        else if (type == "LeftBrace") tokenType = TokenType::LeftBrace;
        else if (type == "RightBrace") tokenType = TokenType::RightBrace;
        else if (type == "Text") tokenType = TokenType::Text;
        else if (type == "Header") tokenType = TokenType::Header;
        else if (type == "LexerError") tokenType = TokenType::LexerError;
        else tokenType = TokenType::EndOfFile;

        tokens.push_back({tokenType, value});
    }

    return tokens;
}

// Parser class
class Parser {
public:
    explicit Parser(const vector<Token> &tokens, ofstream &errorOutput) 
        : tokens(tokens), pos(0), errorOutput(errorOutput) {}

    ASTNode* parse() {
        if (TokenType::LexerError == currentToken().type) {
              //errorOutput << "Parsing cannot be done due to lexer errors." << endl;
             return nullptr;
        }
        ASTNode* root = new ASTNode("Document", "");
        while (currentToken().type != TokenType::EndOfFile && currentToken().type != TokenType::LexerError) { 
            //errorOutput<< "Parsing token: " << currentToken().type << "and" << currentToken().value << endl;
            if (isHeader()) {
                root->children.push_back(parseHeader());
            } else if (isTag()) {
                root->children.push_back(parseTag());
            } else if (isAlert()) {
                root->children.push_back(parseAlert());
            } else if (currentToken().type == TokenType::Text) {
            // Handle standalone text
                root->children.push_back(new ASTNode("Text", currentToken().value));
                advance();
            } //else if (currentToken().type == TokenType::LexerError) {
               // errorOutput << "Parsing cannot be done due to lexer errors." << endl;
                //return nullptr;}
             else {
                syntaxError("Unexpected token: " + currentToken().value);
                return nullptr;
            }
        }
        if(currentToken().type == TokenType::LexerError)
        {
            root=nullptr;
            return nullptr;
        }
        return root;
    }

private:
    vector<Token> tokens;
    size_t pos;
    ofstream &errorOutput;

    Token currentToken() {
        return tokens[pos];
    }

    void advance() {
        if (pos < tokens.size()) pos++;
    }

    bool isHeader() {
        return currentToken().type == TokenType::Header;
    }

    bool isTag() {
        return currentToken().type == TokenType::Tag;
    }

    bool isAlert() {
        return currentToken().type == TokenType::Operator && currentToken().value == "!!";
    }

    // bool containsLexerErrors() {
    //     for (const Token& token : tokens) {
    //         if (curre) {
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    ASTNode* parseHeader() {
        Token token = currentToken();
        advance();
        ASTNode* headerNode = new ASTNode("Header", token.value);
        if (currentToken().type == TokenType::Text) {
            headerNode->children.push_back(new ASTNode("Text", currentToken().value));
            advance();
        } else {
            syntaxError("Expected text after header");
        }
        return headerNode;
    }

    ASTNode* parseTag() {
        Token token = currentToken();
        advance();
        ASTNode* tagNode = new ASTNode("Tag", token.value);

        // Parse class name inside []
        if (currentToken().type == TokenType::Lbracket) {
            tagNode->children.push_back(new ASTNode("Lbracket", "["));
            advance();
            if (currentToken().type == TokenType::ClassName) {
                ASTNode* classNameNode = new ASTNode("ClassName", currentToken().value);
                tagNode->children.push_back(classNameNode); // Indented child
                advance();
                if (currentToken().type == TokenType::Rbracket) {
                    tagNode->children.push_back(new ASTNode("Rbracket", "]"));
                    advance();
                } else {
                    syntaxError("Expected closing bracket ]");
                }
            } else {
                syntaxError("Expected class name inside brackets");
            }
        }

        // Parse attributes inside {}
        if (currentToken().type == TokenType::LeftBrace) {
            tagNode->children.push_back(new ASTNode("LeftBrace", "{"));
            advance();
            if (currentToken().type == TokenType::Attribute) {
                ASTNode* attributeNode = new ASTNode("Attribute", currentToken().value);
                tagNode->children.push_back(attributeNode); // Indented child
                advance();
                if (currentToken().type == TokenType::RightBrace) {
                    tagNode->children.push_back(new ASTNode("RightBrace", "}"));
                    advance();
                } else {
                    syntaxError("Expected closing brace }");
                }
            } else {
                syntaxError("Expected attribute inside braces");
            }
        }

        return tagNode;
    }

    ASTNode* parseAlert() {
        ASTNode* alertNode = new ASTNode("Alert", "!!");
        advance();
        if (currentToken().type == TokenType::Text) {
            alertNode->children.push_back(new ASTNode("Text", currentToken().value));
            advance();
        } else {
            syntaxError("Expected text after alert operator !!");
        }
        return alertNode;
    }

    void syntaxError(const string &message) {
        x=1;
        errorOutput << "Syntax error: " << message << " at position " << pos << endl;
    }
};

// Function to write the AST to a file in a tree structure
void writeAST(ASTNode* node, ofstream &file, int indent = 0) {
    if(node==nullptr)
    {
        file << "Parsing failed due to lexer errors." << endl;
    }
    for (int i = 0; i < indent; ++i) file << "  ";
    file << node->type << ": " << node->value << endl;

    for (auto child : node->children) {
        writeAST(child, file, indent + 1);
    }
}

int main() {
    vector<Token> tokens = readTokens("output.txt");

    ofstream outputFile("parser_output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open parser_output.txt for writing." << endl;
        return 1;
    }

    Parser parser(tokens, outputFile);

    ASTNode* ast = parser.parse();
    if(ast==nullptr)
    {
        outputFile << "Parsing failed due to lexer errors." << endl;
        writeAST(ast, outputFile);
    }
    if (ast) {
        if (x==0)
            writeAST(ast, outputFile);
        else
            outputFile << "Parsing failed due to syntax errors." << endl;
    } else {
        outputFile << "Parsing failed due to syntax errors." << endl;
    }

    outputFile.close();
    delete ast;
    return 0;
}