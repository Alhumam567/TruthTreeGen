#ifndef REGEX_H
    #include <regex>
#endif

#ifndef UTIL_H
#define UTIL_H

class DecompositionUtil
{
    enum class OP_PREC{ UNIVERSAL=0, EXIST=0, NOT=1, AND=2, OR=3, COND=4, BICOND=4, ERR=-1 };

private:
    static bool hasHigherPrecendence(const std::string &oldConn, const std::string &newConn);
    static OP_PREC getOperatorPrecendence(const std::string &conn);
    static int findMainConnective(const std::string &statement, std::string *mainConnective);

public:
    static bool decompose(const std::string &statement, std::vector<std::string> *decomposedStatement);
    static std::size_t strlen_utf8(const std::string &str); 
    static bool isLiteral(const std::string &statement); 
};

#endif