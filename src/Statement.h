#ifndef STATEMENT_H
#define STATEMENT_H
#include <iostream>

struct Statement {
    std::string value;              // The statement string
    std::string mc;                 // The main connective in this statement
    short mc_pos;                   // Main Connective position in <value> 
    bool isLiteral; 
};

#endif