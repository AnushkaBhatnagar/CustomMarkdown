#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

enum class TokenType {
    Operator,
    Tag,
    ClassName,
    Attribute,
    Lbracket,
    Rbracket,
    LeftBrace,
    RightBrace,
    Text,
    Header,
    LexerError,
    EndOfFile,
};

struct Token {
    TokenType type;
    std::string value;
};

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        exit(1);
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

class Lexer {
public:
    explicit Lexer(const std::string &input) : input(input), pos(0) {}

    Token nextToken() {
        int c=0;
        while (pos < input.length()) {
            char current = input[pos];

            // Skip whitespace
            if (std::isspace(current)) {
                pos++;
                continue;
            }

            // Handle headers like #, ##, ###, ####
            if (current == '#') {
                std::string header;
                while (current == '#' && pos < input.length()) {
                    header += current;
                    pos++;
                    current = input[pos];
                }
                if (header.length() <= 4) {
                    return {TokenType::Header, header};
                } else {
                    logError("Invalid header size: " + header);
                }
            }

            // Handle operators like **, *, >, !!, ?, =, :
            if (current == '*' || current == '-' || current == '>' || current == '!' || current == '?' || current == '=' || current == ':') {
                std::string op(1, current);
                pos++;
                if (current == '!' && pos < input.length() && input[pos] == '!') { // For !!
                    op += "!";
                    pos++;
                }
                return {TokenType::Operator, op};
            }

            // Handle tags starting with @
            if (current == '@') {
                pos++;
                std::string tag;
                while (pos < input.length() && std::isalnum(input[pos])) {
                    tag += input[pos++];
                }
                return {TokenType::Tag, tag};
            }

            // Handle brackets []
            if (current == '[') {
                pos++;
                return {TokenType::Lbracket, "["};
            }
            if (current == ']') {
                pos++;
                return {TokenType::Rbracket, "]"};
            }

            // Handle braces {}
            if (current == '{') {
                pos++;
                return {TokenType::LeftBrace, "{"};
            }
            if (current == '}') {
                pos++;
                return {TokenType::RightBrace, "}"};
            }

             if (input[pos - 1] == '[') {
                std::string className;
                c=0;
                while (pos < input.length() && input[pos] != ']') {
                    className += input[pos++];
                    c++;
                }
                if(input[pos] == ']')
                {
                    return {TokenType::ClassName, className};
                }
                else
                {
                    className="";
                    pos=pos-c;
                }
            }

            // Handle attributes inside {}
            if (input[pos - 1] == '{') {
                std::string attribute;
                c=0;
                while (pos < input.length() && input[pos] != '}') {
                    attribute += input[pos++];
                    c++;
                }
                if(input[pos] == '}')
                {
                    return {TokenType::Attribute, attribute};
                }
                else
                {
                    attribute="";
                    pos=pos-c;
                }
            }

            // Handle text inside " "
            if (current == '"') {
                pos++;
                std::string text;
                c=0;
                while (pos < input.length() && input[pos] != '"') {
                    text += input[pos++];
                    c++;
                }
                if(input[pos] == '"')
                {
                    pos++; // Skip closing "
                    return {TokenType::Text, text};
                }
                else
                {
                    text="";
                    pos=pos-c-1;
                }
            }
            else
            {
                // If unknown character, log error
               // logError("Unknown character: " + std::string(1, current));
                pos++;
                return {TokenType::LexerError, "Unknown character: " + std::string(1, current)};
            }
        }
        return {TokenType::EndOfFile, ""};
    }

private:
    std::string input;
    size_t pos;

    void logError(const std::string& message) {
        std::ofstream errorFile("output.txt", std::ios::app);
        if (errorFile.is_open()) {
            errorFile << "Error: " << message << std::endl;
            //return {TokenType::LexerError, message};
            errorFile.close();
        } else {
            cerr << "Error: Could not open output file for logging errors." << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string input = readFile(argv[1]);

    // Clear previous output
    ofstream outputFile("output.txt");
    outputFile.close(); // Just to clear the file initially

    outputFile.open("output.txt", ios::app);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file." << endl;
        return 1;
    }

    Lexer lexer(input);
    Token token;

    do {
        token = lexer.nextToken();
        std::string type;
        switch (token.type) {
            case TokenType::Operator: type = "Operator"; break;
            case TokenType::Tag: type = "Tag"; break;
            case TokenType::ClassName: type = "ClassName"; break;
            case TokenType::Attribute: type = "Attribute"; break;
            case TokenType::Lbracket: type = "Lbracket"; break;
            case TokenType::Rbracket: type = "Rbracket"; break;
            case TokenType::LeftBrace: type = "LeftBrace"; break;
            case TokenType::RightBrace: type = "RightBrace"; break;
            case TokenType::Text: type = "Text"; break;
            case TokenType::Header: type = "Header"; break;
            case TokenType::EndOfFile: type = "EndOfFile"; break;
            case TokenType::LexerError: type = "LexerError"; break;
        }
        outputFile << "<" << type << ", " << token.value << ">" << std::endl;
    } while (token.type != TokenType::EndOfFile);

    outputFile.close();
    return 0;
}
