#include "assembler.hpp"

#define INST "_instructions" // file location of instruction set for assembler
#define EXT ".hack" // file extension of output file

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

  while (parser.hasMoreLines == true){
    parser.advance();

    type type = parser.instructionType();

    if (type == A_INSTRUCTION){
      out << "0";
      string binaryRep = std::bitset<15>(stoi(parser.symbol())).to_string(); // assume no errors in @ (like text)
      out << binaryRep << endl;
      // translate into binary
    } else if (type == L_INSTRUCTION){
      // part of advanced assembler
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
