#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stack>

using namespace std;
int z=0;
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

// Helper function to log errors
void logError(const string& message, ofstream& logFile) {
    z=1;
    logFile << "Error: " << message << endl;
}

// Function to generate HTML from AST
string generateHTML(ASTNode* node, ofstream& logFile) {
    if (!node) {
        logError("Null node encountered", logFile);
        return "";
    }

    stringstream html;

    if (node->type == "Header") {
        if (node->children.empty()) {
            logError("Header tag missing text", logFile);
            return "";
        }
        string headerType = node->value;
        logFile<<"Header type is "<<headerType<<endl;
        if (headerType == "##") {
            html << "<h2>" << node->children[0]->value << "</h2>";
        } else if (headerType == "#") {
            html << "<h1>" << node->children[0]->value << "</h2>";
        } else if (headerType == "###") {
            html << "<h3>" << node->children[0]->value << "</h3>";
        } else {
            logError("Unknown header type: " + headerType, logFile);
            return "";
        }
    } 
    else if (node->type == "Header") {
        string headerTag;
        if (node->value == "##") {
            headerTag = "h2";
        } else if (node->value == "#") {
            headerTag = "h1";
        } else if (node->value == "###") {
            headerTag = "h3";
        } else {
            headerTag = "h4"; // Default to <h4> for unknown header levels
        }

        html<<"<" + headerTag + ">";
        for (ASTNode* child : node->children) {
            html<<generateHTML(child, logFile);
        }
        html<<"</" + headerTag + ">";
    }
    else if (node->type == "Tag") {
        string tagName = node->value;
        html << "<" << tagName.substr(1);

        bool hasClass = false;
        bool hasAttributes = false;

        // Process class name
        for (auto child : node->children) {
            if (child->type == "ClassName") {
                html << " class=" << child->value << "";
                hasClass = true;
            }
            if (child->type == "Attribute") {
                if (!hasAttributes) {
                    html << " ";
                }
                html << child->value;
                hasAttributes = true;
            }
        }

        html << ">";

        // Process the content inside the tag
        for (auto child : node->children) {
            if (child->type == "Text") {
                html << child->value;
            }
        }

        html << "</" << tagName.substr(1) << ">";
    } else {
        logError("Unexpected node type: " + node->type, logFile);
        return "";
    }

    return html.str();
}

// Function to parse AST content from the file and build the AST tree
vector<ASTNode*> parseAST(const string& astContent, ofstream& logFile) {
    vector<ASTNode*> astNodes;
    stringstream ss(astContent);
    string line;
    stack<ASTNode*> nodeStack;

    logFile << "Starting AST Parsing..." << endl;

    while (getline(ss, line)) {
        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        logFile << "Processing line: " << line << endl;

        if (line.find("Document") != string::npos) {
            ASTNode* rootNode = new ASTNode("Document", "");
            logFile << "Created root node: Document" << endl;
            nodeStack.push(rootNode);
          //  astNodes.push_back(rootNode);  //CHANGE IT
            //logFile<<"---------------"<<nodeStack.top()->type<<endl;
         
            while(getline(ss, line)) {
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);

                if (line.empty()) continue;

                    if (line.find("Header") != string::npos) {
                    size_t pos = line.find(",") + 2;
                    string value = line.substr(pos, line.find("}") - pos);
                    ASTNode* headerNode = new ASTNode("Header", value);
                    logFile << "Created node: Type = Header, Value = " << value << endl;
                    if (!nodeStack.empty()) nodeStack.top()->children.push_back(headerNode);
                    nodeStack.push(headerNode);
                    astNodes.push_back(headerNode);  
                    //logFile<<"---------------"<<nodeStack.top()->type<<endl;
                }
                else if (line.find("Text") != string::npos) {
                    size_t pos = line.find(",") + 2;
                    string value = line.substr(pos, line.find("}") - pos);
                    ASTNode* textNode = new ASTNode("Text", value);
                    logFile << "Created node: Type = Text, Value = " << value << endl;
                    if (!nodeStack.empty()) nodeStack.top()->children.push_back(textNode);
                    astNodes.push_back(textNode);  
                   // logFile<<"---------------"<<nodeStack.top()->type<<endl;
                }
                else if (line.find("Tag") != string::npos) {
                    size_t pos = line.find(",") + 1;
                    string value = line.substr(pos, line.find("}") - pos);
                    ASTNode* tagNode = new ASTNode("Tag", value);
                    logFile << "Created node: Type = Tag, Value = " << value << endl;
                    if (!nodeStack.empty()) nodeStack.top()->children.push_back(tagNode);
                    nodeStack.push(tagNode);
                   // logFile<<"---------------"<<nodeStack.top()->type<<endl;
                    streampos linepos = ss.tellg(); 
                    string lineinsidetag;           
                    int c=0; //counter for checking if tag is closed or not    
                    while(getline(ss, lineinsidetag)) { //Inner While for class and attributes
                        lineinsidetag.erase(0, lineinsidetag.find_first_not_of(" \t"));
                        lineinsidetag.erase(lineinsidetag.find_last_not_of(" \t") + 1);

                        if (lineinsidetag.empty()) continue;
                        if(lineinsidetag.find("Lbracket") != string::npos
                        ||lineinsidetag.find("Rbracket") != string::npos
                        ||lineinsidetag.find("LeftBrace") != string::npos
                        ||lineinsidetag.find("RightBrace") != string::npos)
                        {
                            c=1;
                            continue;
                        }

                        if (lineinsidetag.find("ClassName") != string::npos) {
                            size_t pos = lineinsidetag.find(",") + 1;
                            string value = lineinsidetag.substr(pos, lineinsidetag.find("}") - pos);
                            ASTNode* classNode = new ASTNode("ClassName", value);
                            logFile << "Created node: Type = ClassName, Value = " << value << endl;
                            if (!nodeStack.empty()) nodeStack.top()->children.push_back(classNode);
                            // astNodes.push_back(classNode);  //CHANGE IT
                            //  logFile<<"---------------"<<nodeStack.top()->type<<endl;
                            c=1;
                            continue;
                        }
                        else if (lineinsidetag.find("Attribute") != string::npos) {
                            size_t pos = lineinsidetag.find(",") + 1;
                            string value = lineinsidetag.substr(pos, lineinsidetag.find("}") - pos);
                            ASTNode* attributeNode = new ASTNode("Attribute", value);
                            logFile << "Created node: Type = Attribute, Value = " << value << endl;
                            if (!nodeStack.empty()) nodeStack.top()->children.push_back(attributeNode);
                        //    logFile<<"---------------"<<nodeStack.top()->type<<endl;  
                            c=1;  
                            continue;    
                        }
                        else if (lineinsidetag.find("Text") != string::npos) {
                            size_t pos = lineinsidetag.find(",") + 2;
                            string value = lineinsidetag.substr(pos, lineinsidetag.find("}") - pos);
                            ASTNode* attributeNode = new ASTNode("Text", value);
                            logFile << "Created node: Type = Text, Value = " << value << endl;
                            if (!nodeStack.empty()) nodeStack.top()->children.push_back(attributeNode);
                        //    logFile<<"---------------"<<nodeStack.top()->type<<endl;  
                            c=1;  
                            continue;    
                        }
                        else if (lineinsidetag.find("Header") != string::npos) {
                            size_t pos = lineinsidetag.find(",") + 2;
                            string value = lineinsidetag.substr(pos, lineinsidetag.find("}") - pos);
                            ASTNode* attributeNode = new ASTNode("Header", value);
                            logFile << "Created node: Type = Header, Value = " << value << endl;
                            if (!nodeStack.empty()) nodeStack.top()->children.push_back(attributeNode);
                        //    logFile<<"---------------"<<nodeStack.top()->type<<endl;  
                            c=1;  
                            continue;    
                        }
                        else if(lineinsidetag.find("}") != string::npos&&c==1) {
                            c=0;
                            continue;
                        }
                        else if(lineinsidetag.find("}") != string::npos&&c==0) {
                            break;
                        }
                } //Inner while closes
                //logFile<<"Executed tag now"<<endl;
                astNodes.push_back(tagNode);  //CHANGE IT
                } // TAG CLOSING
                else if (line.find("}") != string::npos&&nodeStack.top()->type!="Document") {
                    if (!nodeStack.empty()) {
                        ASTNode* completedNode = nodeStack.top();
                        nodeStack.pop();
                        //logFile << "Closed node: Type = " << completedNode->type << endl;
                        if (nodeStack.empty()) {
                        //    logFile << "Root node completed. Adding to AST." << endl;
                            astNodes.push_back(completedNode);
                        }
                    } else {
                        logError("Unmatched closing brace in AST", logFile);
                    }
                }
            }
        }
        if (line.find("}") != string::npos&&nodeStack.top()->type=="Document") {
            if (!nodeStack.empty()) {
                ASTNode* completedNode = nodeStack.top();
                nodeStack.pop();
                if (nodeStack.empty()) {
                  //  logFile << "Root node completed. Adding to AST." << endl;
                        astNodes.push_back(completedNode);
                    }
            } else {
                    logError("Unmatched closing brace in AST", logFile);
            }
        }
    }

   // logFile << "AST Parsing completed. Number of nodes in AST: " << astNodes.size() << endl;

    return astNodes;
}

// Main function to generate the HTML file from AST
void generateHTMLFromAST(const string& astFile, const string& outputFile, const string& errorLogFile) {
    ifstream inputFile(astFile);
    ofstream output(outputFile);
    ofstream logFile(errorLogFile);

    if (!inputFile.is_open()) {
        logFile << "Error: Could not open AST file" << endl;
        return;
    }

    if (!output.is_open()) {
        logFile << "Error: Could not open output HTML file" << endl;
        return;
    }

    if (!logFile.is_open()) {
        cerr << "Error: Could not open error log file" << endl;
        return;
    }

    stringstream astContent;
    string line;
    while (getline(inputFile, line)) {
        astContent << line << endl;
    }

    // Parse the AST content
    vector<ASTNode*> astNodes = parseAST(astContent.str(), logFile);

    // Start HTML generation
    output << "<!DOCTYPE html>"<<endl<<"<html>"<<endl<<"<head>"
    <<endl<<"<title>Generated HTML from CustomMarkdown</title>"
    <<endl<<"<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css' rel=stylesheet crossorigin='anonymous'>"
    <<endl<<"<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js' crossorigin='anonymous'></script>"
    <<endl<<"</head>"
    <<endl<<"<body>"<<endl;

    for (ASTNode* node : astNodes) {
        string html = generateHTML(node, logFile);
        if (!html.empty()) {
            output << html<<endl;
        }
    }

    output<<"</body>"<<endl<<"</html>";
    logFile << "HTML generation completed successfully." << endl;
   // logFile<<html;

    inputFile.close();
    output.close();
    logFile.close();
}

int main() {
    if(z==1)
    {
        cout<<"Error in Code Generation Phase"<<endl;
        return 1;
    }
    else
    generateHTMLFromAST("parser_output.txt", "output.html", "log.txt");
    return 0;
}
