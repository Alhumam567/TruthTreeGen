#ifndef REGEX_H
    #include <regex>
#endif

#ifndef UTIL_H
#define UTIL_H

class DecompositionUtil
{
private:
    enum class OP_PREC{ UNIVERSALEXIST=0, NOT=1, AND=2, OR=3, COND=4, BICOND=4, ERR=-1 };
    static bool hasHigherPrecendence(std::string oldConn, std::string newConn);
    static OP_PREC getOperatorPrecendence(std::string conn);

public:
    static std::string *decompose(std::string &statement);
    static std::string findMainConnective(std::string &statement);
};



#endif