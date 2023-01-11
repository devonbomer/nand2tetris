#include "assembler.hpp"

#define INST "_instructions" // file location of instruction set for assembler
#define TABLE "_symbols"
#define EXT ".hack"          // file extension of output file
#define VAR 16             // starting variable RAM space

using namespace std; // TODO: remove namespace dependency

int main(int argc, char** argv){
  
  // check if file has .asm extension
  if(argc < 2){
    cout << "No filename given.\n";
    return 0;
  } else if(argc > 2){
    cout << "Too many arguments specified.\n";
    return 0;
  }

  // TODO: make into function, arg:char *, string extension. Output: bool true if match
  const char * arg = argv[1];
  string s(arg);
  string ext;
  
  // TODO: replace with new implementation of string
  for(int i = 4;i > 0; i--){
    ext = ext + s[s.length()-i];
  }
  // check extension is .asm
  if(ext != ".asm"){
    cout << "Not an appropriate extension.\n";
    return 0;
  }
  // remove asm from variable
  for(int i = 4; i > 0; i--){
    s.pop_back();
  }

  // assign .hack to output name and make a new file
  string repext = EXT;
  string outname = s + repext;
  string source(arg);
  
  ifstream in;
  ofstream out; //ofstream overwrites file if already present

  in.open(source);
  out.open(outname);

  Parser parser(&in);
  Coder coder(INST);
  symboltable symboltable(TABLE);

  // PASS 1
  int lineNumber = 0;

  while (parser.hasMoreLines == true){
    parser.advance(); // advance will skip comment and empty lines

    type type = parser.instructionType();
    if(type == A_INSTRUCTION || type == C_INSTRUCTION){ // CHECK FOR A INSTRUCTION THAT ISN'T A VARIABLE?
      lineNumber++;
    }

    if (type == L_INSTRUCTION){
      std::string symbol = parser.symbol();
      char firstChar = symbol[0];
        if(!symboltable.contains(symbol)){
          symboltable.addEntry(symbol,lineNumber);
        }
      }
  }
  
  // reset input filestream pointer
  in.clear();
  in.seekg(0);
  parser.hasMoreLines = true;

  // PASS 2
  int RAM = VAR;

  while (parser.hasMoreLines == true){
    parser.advance();

    type type = parser.instructionType();

    if (type == A_INSTRUCTION){
      out << "0";
      // track first digit:
      std::string symbol = parser.symbol();
      char firstChar = symbol[0];

      if(std::isdigit(firstChar)){ // is number
        out << std::bitset<15>(stoi(symbol)).to_string() << endl; // assume no errors in @ (like text)
      } else { // is a variable name
              // check symbol table for variable name, insert if not there, replace with value if key found
          if(!symboltable.contains(symbol)){
            symboltable.addEntry(symbol, RAM);
            RAM++;                               // need to track available ram space for variables
          }

        out << std::bitset<15>(symboltable.getAddress(symbol)) << endl;
        cout << symbol.length() << ":" << std::bitset<15>(symboltable.getAddress(symbol)) << endl;
      }

    } else if (type == C_INSTRUCTION){
      out << "111";

      if(parser.comp().find("M") != string::npos){
        out << "1";
      } else {
        out << "0";
      }
      out << coder.comp(parser.comp()) << coder.dest(parser.dest()) << coder.jump(parser.jump()) << endl;
    }
  }
  out.close();
  in.close();
}
