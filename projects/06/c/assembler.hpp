#include "parser.hpp"
#include "coder.hpp" // small enough not to worry about precompiled headers
#include "symboltable.hpp"

//TODO: is it necessary to define assembler as it's own class? Completely composed of other header file elements
class assembler{
    public:
        assembler();
        void translate();
};