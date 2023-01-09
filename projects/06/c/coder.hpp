#include <bitset>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Coder{
    public:
        std::ifstream instructions;
        
        Coder(string inst_loc);
        ~Coder();
        std::string dest(std::string field);
        std::string comp(std::string field);
        std::string jump(std::string field);
};

Coder::Coder(string inst_loc){
    instructions.open(inst_loc);
}
Coder::~Coder(){
    instructions.close();
}

std::string Coder::dest(std::string field){
    instructions.clear();
    instructions.seekg(0);
    std::string line;
    int8_t destFlag = 0;

    while(std::getline(instructions,line)){ // narrow down the line of the instruction
        // set destination flag
        if (line.find("DEST") != string::npos){
            destFlag = 1;
        } else if (line.find("COMP") != string::npos){
            destFlag = 0;
        } else if (line.find("JUMP") != string::npos){
            destFlag = 0;
        }

        // find matching opcode
        if (line.find(field) != string::npos && destFlag == 1){
            size_t des = line.find(',');
            string ln = line.substr(0,des);

            if (ln == field){
                string bin = line.substr(des + 1,line.length());
                return bin;
            }
        }
    }

    return "ERROR";
}

std::string Coder::comp(std::string field){
    instructions.clear();
    instructions.seekg(0);

    std::string line;
    int8_t compFlag = 0;

    while(std::getline(instructions,line)){ // narrow down the line of the instruction
        // set destination flag
        if (line.find("DEST") != string::npos){
            compFlag = 0;
        } else if (line.find("COMP") != string::npos){
            compFlag = 1;
        } else if (line.find("JUMP") != string::npos){
            compFlag = 0;
        }

        // find matching opcode
        if (line.find(field) != string::npos && compFlag == 1){
            size_t des = line.find(",");
            if (des == string::npos){
               // cout << "FAILED" << line << endl;
            }
            string ln = line.substr(0,des);
            if (ln == field){
                string bin = line.substr(des + 1, line.length());
                return bin;
            }
        }
    }
   
    return "ERROR";
}

std::string Coder::jump(std::string field){
    instructions.clear();
    instructions.seekg(0);
    std::string line;
    int8_t jumpFlag = 0;

    while(std::getline(instructions,line)){ // narrow down the line of the instruction
    
        // set destination flag
        if (line.find("DEST") != string::npos){
            jumpFlag = 0;
        } else if (line.find("COMP") != string::npos){
            jumpFlag = 0;
        } else if (line.find("JUMP") != string::npos){
            jumpFlag = 1;
        }

        // find matching opcode
        if (line.find(field) != string::npos && jumpFlag == 1){
            size_t des = line.find(',');
            string ln = line.substr(0,des);

            if (ln == field){
                string bin = line.substr(des + 1,line.length());
                //cout << "bin: " << bin << endl;
                return bin;
            }
        }
    }

    return "ERROR";
}