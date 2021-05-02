#include <iostream>
#include "TruthTreeGen.h"

class DecompositionUtil
{
public:
    static std::string *decompose(std::string &statement);

private:
    static std::string findMainConnective(std::string &statement);
};

std::string *DecompositionUtil::decompose(std::string &statement)
{
    std::string mainConn = DecompositionUtil::findMainConnective(statement);

    if (mainConn == specialChars[0]) {

    }
}

std::string DecompositionUtil::findMainConnective(std::string &statement)
{
    
}