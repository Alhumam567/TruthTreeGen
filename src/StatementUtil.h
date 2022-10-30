#ifndef UTIL_H
#define UTIL_H

#include <regex>
#include <unordered_map>

struct Statement {
    std::string value;              // The statement string
    std::string mc;                 // The main connective in this statement
    short mc_pos;                   // Main Connective position in <value> 
    bool isLiteral; 
};

enum class OP;
extern std::unordered_map<std::string, OP> op_mapping;

class StatementUtil
{
private:
    static bool hasHigherPrecendence(const std::string &oldConn, const std::string &newConn);
    static OP getOperatorPrecendence(const std::string &conn);
    static int findMainConnective(Statement &statement);

public:
    static Statement initializeStatement(const std::string &str, bool verbose);
    static bool decompose(const Statement &statement, std::vector<Statement> *decomposedStatement);
    static void addNegation(Statement &statement);
    static bool isLiteral(Statement &statement); 
    static bool isNegations(const std::string &literal1, const std::string &literal2);
    static std::size_t strlen_utf8(const std::string &str); 
};

#endif