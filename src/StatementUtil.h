#ifndef REGEX_H
    #include <regex>
#endif

#ifndef UTIL_H
#define UTIL_H

struct Statement {
    std::string value;              // The statement string
    std::string mc;                 // The main connective in this statement
    short mc_pos;                   // Main Connective position in <value> 
    bool isLiteral; 
};

class DecompositionUtil
{
    enum class OP_PREC{ UNIVERSAL=0, EXIST=0, NOT=1, AND=2, OR=3, COND=4, BICOND=4, ERR=-1 };

private:
    static bool hasHigherPrecendence(const std::string &oldConn, const std::string &newConn);
    static OP_PREC getOperatorPrecendence(const std::string &conn);
    static int findMainConnective(Statement &statement);

public:
    static Statement initializeStatement(const std::string &str);
    static bool decompose(const Statement &statement, std::vector<Statement> *decomposedStatement);
    static void addNegation(Statement &statement);
    static bool isLiteral(Statement &statement); 
    static bool isNegations(const std::string &literal1, const std::string &literal2);
    static std::size_t strlen_utf8(const std::string &str); 
};

#endif