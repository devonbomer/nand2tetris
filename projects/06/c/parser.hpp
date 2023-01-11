#ifndef PARSER
#define PARSER 1
#include <iostream>
#include <fstream>
#include <string>
using namespace std; // avoid in future, say using in front of first call so don't have to repeat std::

enum type{A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION};

// Parser is responsible for converting the current line's symbols into filtered and pure instruction parts.
class Parser{
    public:
        ifstream* file;
        string currentLine;
        Parser(ifstream* f);

        bool hasMoreLines;
        type instructionType();
        void advance();
        string symbol();
        string dest();
        string comp();
        string jump();
};


string cleanString(string input){

    // search for and remove trailing comments:
    size_t cmt = input.find('/');
    if (cmt != string::npos){
        input = input.substr(0, cmt);
    }

    // remove blank / tab space before and after input:
    while((input[input.length() -1] == 0x20 || input[input.length() -1] == 0x09) && input.length() > 0){ // check for space between = and last character and tab character
        input.pop_back();
        }
    while((input[0] == 0x20 || input[0] == 0x09) && input.length() > 0){ // untested
        input.erase(input.begin());
        }
    while((input[input.length() - 1] == 0x0D || input[input.length() - 1] == 0x09) && input.length() > 0){
        input.pop_back();
    }
    while((input[0] == 0x0D || input[0] == 0x09) && input.length() > 0){ // untested
        input.erase(input.begin());
    }
    return input;
}

Parser::Parser(ifstream* f){
    this->file = f;
    if(file->peek() == EOF){
        cout << "File is empty\n"; // TODO: check other conditions later
        this->hasMoreLines = false;
    } else {
        this->hasMoreLines = true;
    }
};

type Parser::instructionType(){
    char firstChar = currentLine[0];
    
    if(firstChar == 0x40){          // @ sign
        return A_INSTRUCTION;
    } else if(firstChar == 0x28){   // (
        return L_INSTRUCTION;
    } else {                        
        return C_INSTRUCTION;
    }

    return A_INSTRUCTION;           //assuming no error reporting, default to C
};

void Parser::advance(){ 
// setter for currentLine. skip white space, empty lines, and trailing / leading comments
    if(this->hasMoreLines == true){
        std::getline(*file, currentLine);
        // remove surrounding spaces
        this->currentLine = cleanString(currentLine); 
        char firstChar = currentLine[0];

        // check if blank line after blanks are removed: getline removes delimiter in the input so just need to check if line is empty
        if(firstChar == 0x20 || firstChar == 0x2F || firstChar == 0x5C || firstChar == 0x0a || firstChar == 0x0D || firstChar == 0x09 || currentLine.empty()){ // 0x20 = " " / blank space
            advance();
        }

        if(file->peek() == EOF){
            this->hasMoreLines = false;
        }

    } else { // called with no lines left
        cout << "EOF\n";
    }
};

string Parser::symbol(){
    // removes parenthesis or @ from symbol instruction
    string symbol;

    if(currentLine[0] == '('){
      symbol = this->currentLine.substr(1, this->currentLine.size() - 2);
    } else if (currentLine[0] == '@'){
        symbol = this->currentLine.substr(1, this->currentLine.size());
    } else {
        cout << "failed to find matching Symbol on line containing " << currentLine << endl;
        return "null";
    }

    return cleanString(symbol);
}

string Parser::dest(){
    // check for equal sign, if none then dest is null and return null
    size_t eq = currentLine.find('=');
    if(eq == string::npos){                     // if no equal sign in instruction
        return "null";
    } else {
        string c = currentLine.substr(0, eq);   // return up to the equal sign
        return cleanString(c);
    }
}

std::string Parser::comp(){
    // Doesn't manipulate currentLine directly. Returns comp component of C_INSTRUCTION. Comp is mandatory.

    size_t eq = currentLine.find('=');
    std::string c;

    if(eq != string::npos){                                     // if equal exists
        c = currentLine.substr(eq + 1, currentLine.length());   // redefine c, remove dest, else leave the first half in tact
    } else {
        c = currentLine;                                        // initialize c without = sign
    }

    size_t col = c.find(";");                                   // if semicolon in remaining output
    if(col != string::npos){
        c = c.substr(0, col);
    }

    return cleanString(c);                                      // remove any surrounding spaces
}

std::string Parser::jump(){
 size_t col = currentLine.find(';');
    if(col == string::npos){                                        // if no equal sign in instruction
        return "null";
    } else {
        string c = currentLine.substr(col + 1, currentLine.size()); // return up to the equal sign
        return cleanString(c);
    }
}

#endif