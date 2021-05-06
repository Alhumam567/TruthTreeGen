#ifndef REGEX_H
    #include <regex>
#endif

#ifndef UTIL_H
#define UTIL_H

class DecompositionUtil
{
    enum class OP_PREC{ UNIVERSAL=0, EXIST=0, NOT=1, AND=2, OR=3, COND=4, BICOND=4, ERR=-1 };

private:
    static bool hasHigherPrecendence(std::string &oldConn, std::string &newConn);
    static OP_PREC getOperatorPrecendence(std::string &conn);
    static int findMainConnective(const std::string &statement, std::string *mainConnective);

public:
    static bool decompose(std::string &statement, std::vector<std::string> *decomposedStatement); 
};

#endif