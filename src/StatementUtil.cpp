#include <iostream>
#include <codecvt>
#include <string>
#include <regex>

#include "TruthTreeGen.h"
#include "StatementUtil.h"

std::size_t DecompositionUtil::strlen_utf8(const std::string& str) {
	std::size_t length = 0;
	for (char c : str) {
		if ((c & 0xC0) != 0x80) {
			++length;
		}
	}
	return length;
}

Statement DecompositionUtil::initializeStatement(const std::string &str) {
    Statement s {str};

    DecompositionUtil::findMainConnective(s);
    DecompositionUtil::isLiteral(s);

    std::cout << "Initializing statement: " << str << "\n";
    std::cout << "\t mc: " << s.mc << "\n";
    std::cout << "\t mc_pos: " << s.mc_pos << "\n";
    std::cout << "\t isLiteral: " << s.isLiteral << "\n";

    return s;
}

/** Breaks down <statement> using its main connective and stores the resulting decomposition in <decomposedStatement>.
 *  Returns whether the decomposition rule of the main connective branches:
 *      Conj:          Neg Conj:
 *       P^Q            ~(P^Q)
 *        |              /   \
 *        P             ~P   ~Q
 *        Q  
 * 
 *      Disj:          Neg Disj:
 *       PvQ            ~(PvQ)
 *       / \               |
 *      P   Q             ~P
 *                        ~Q
 *  
 *      Cond:          Neg Cond:
 *      P->Q            ~(P->Q)
 *      /  \               |
 *     ~P   Q              P
 *                        ~Q
 * 
 *     Bicond:        Neg Bicond:
 *      P<->Q          ~(P<->Q)
 *      /   \            /   \
 *      P   ~P           P   ~P
 *      Q   ~Q          ~Q    Q
 */ 
bool DecompositionUtil::decompose(const Statement &statement, std::vector<Statement> *decomposedStatement)
{
    bool split;
    OP_PREC op = DecompositionUtil::getOperatorPrecendence(statement.mc);

    Statement left {DecompositionUtil::initializeStatement(statement.value.substr(0, statement.mc_pos))};
    Statement right {DecompositionUtil::initializeStatement(statement.value.substr(statement.mc_pos+3, 
                                                                                    statement.value.length() - (statement.mc_pos+3)))}; // start at pos+3 bc main connectives are 3 bytes long

    // (Some) Invalid statement cases:
    //      - Left is empty string even though mainconnective is not negation
    //      - Right is empty string
    if ((left.value.length() == 0 && statement.mc != "\uFFE2") || right.value.length() == 0) {
        decomposedStatement->resize(1);
        decomposedStatement->at(0) = {"error, invalid statement"};
        return false;
    }

    // Strip any unnecessary brackets
    int b {0};
    bool noB {left.value[0] != '('};
    for (int i {0}; i < left.value.length(); i++) {
        if (left.value[i] == '(') b++;
        else if (left.value[i] == ')') b--;
        if (b == 0 && i < left.value.length() - 1) { noB = true; break; }
    }
    if (!noB) left = DecompositionUtil::initializeStatement(left.value.substr(1, left.value.length()-2));
    b = 0; noB = right.value[0] != '(';
    for (int i {0}; i < right.value.length(); i++) {
        if (right.value[i] == '(') b++;
        else if (right.value[i] == ')') b--;
        if (b == 0 && i < right.value.length() - 1) { noB = true; break; }
    }
    if (!noB) right = DecompositionUtil::initializeStatement(right.value.substr(1, right.value.length()-2));

    // Decomposing rules
    switch (op) {
        // Universal/Existential Case
        case OP_PREC::UNIVERSAL: {
            if (statement.mc == "\u2200") // Universal
            {}
            else                      // Existential
            {}
            break;
        }
        // And: Split conjuncts in same branch
        case OP_PREC::AND: {
            decomposedStatement->resize(2);

            decomposedStatement->at(0) = left;
            decomposedStatement->at(1) = right;

            split = false;
            break;
        }
        // Or: Split disjuncts into different branches
        case OP_PREC::OR: {
            decomposedStatement->resize(2);

            decomposedStatement->at(0) = left;
            decomposedStatement->at(1) = right;

            split = true;
            break;
        }
        // Not: Depends on inner nested connective
        case OP_PREC::NOT: {
            split = DecompositionUtil::decompose(right, decomposedStatement);

            // Biconditional special case
            if (right.mc == "\u2194") { 
                split = true;
            
                addNegation(decomposedStatement->at(1));
                addNegation(decomposedStatement->at(3));
            } else {
                split = !split;

                for (auto &ds : *decomposedStatement) {
                    // ds = std::string("\uFFE2").append(ds);
                    addNegation(ds);
                }
            }

            break;
        }
        // Conditional/Biconditional case
        case OP_PREC::COND: {
            if (statement.mc == "\u2192") { // Conditional
                decomposedStatement->resize(2);

                decomposedStatement->at(0) = left;         // Negation of Antecedent
                addNegation(decomposedStatement->at(0));
                decomposedStatement->at(1) = right;        // Consequent
            }
            else {                       // Biconditional
                decomposedStatement->resize(4);
                
                // Both True
                decomposedStatement->at(0) = left;
                decomposedStatement->at(1) = right;  

                // Both False
                decomposedStatement->at(2) = left;
                addNegation(decomposedStatement->at(2));
                decomposedStatement->at(3) = right;   
                addNegation(decomposedStatement->at(3));   
            }

            split = true;
            break;
        }
        case OP_PREC::ERR: {
            break;
        }
        
    }

    return split;
}

/** Computes if statement is a literal by first checking if the main connective is either
 *  negation or none, if its none then statement is guaranteed to be literal, if negation
 *  check next character following negation is not an opening bracket.
 */
bool DecompositionUtil::isLiteral(Statement &statement) 
{
    // Main connective is neither a negation or literal
    if (statement.mc != "\uFFE2" && statement.mc != "") { 
        statement.isLiteral = false; 
        return false;
    }

    if (statement.mc == "\uFFE2") {
        std::string innerStatement {statement.value.substr(3, statement.value.length()-3)}; // Ignore negation

        if (innerStatement[0] == '(') { 
            statement.isLiteral = false;
            return false;
        }
    }
    
    statement.isLiteral = true;
    return true;
}

// /** Assumes inputs are literals, simply checks if they are negations of each other */
// bool DecompositionUtil::isNegations(const Statement &literal1, const Statement &literal2) {
//     DecompositionUtil::findMainConnective(literal1);
//     DecompositionUtil::findMainConnective(literal2);

//     if (literal1.mc == "\uFFE2") return literal1.value.substr(3, literal1.value.length()) == literal2.value;
//     else if (literal2.mc == "\uFFE2") return literal2.value.substr(3, literal2.value.length()) == literal1.value;
//     else return false;
// }

/** Assumes inputs are literals, simply checks if they are negations of each other */
bool DecompositionUtil::isNegations(const std::string &literal1, const std::string &literal2) {
    if (literal1.substr(0, 3) == "\uFFE2") return literal1.substr(3, literal1.length()) == literal2;
    else if (literal2.substr(0, 3) == "\uFFE2") return literal2.substr(3, literal2.length()) == literal1;
    else return false;
}

/** This function adds negations to any kind of statement. If a double negation is be created
 *  then the statement will be automatically decomposed to remove the negation. 
 */
void DecompositionUtil::addNegation(Statement &statement) {
    if (!statement.isLiteral) { // If compound
        if (statement.mc == "\uFFE2")
            statement = DecompositionUtil::initializeStatement(statement.value.substr(4, statement.value.length() - 4 - 1)); // Skip neg and opening bracket, don't include outer bracket
        else 
            statement = {"\uFFE2(" + statement.value + ")", "\uFFE2", 0, false};
    } else {                                        // If literal
        if (statement.mc == "\uFFE2")     // Check for existing neg
            statement = {statement.value.substr(3, statement.value.length() - 3), "", -1, true};
        else {
            statement = {"\uFFE2" + statement.value, "\uFFE2", 0, true};
        }
    }
}

int DecompositionUtil::findMainConnective(Statement &statement)
{
    int bracket_c {0}, curr_pos {-1};
    char c;
    std::string utf8_c;
    bool isLogicalConn;

    // Loop over all bytes in the sequence of characters, there could be
    // UTF-8 characters that are variably lengthed
    for(std::size_t i = 0; i < statement.value.length(); i++){
        curr_pos = i;
        c = statement.value[i];
        utf8_c = c;
        isLogicalConn = false;

        // If UTF-8 character
        if ((c & 0x80) == 0x80) {
            utf8_c = c;

            c = statement.value[++i];
            while ((c & 0xC0) == 0x80) {
                utf8_c.append(std::string(1, c));
                c = statement.value[++i];
            }
            i--;

            if (DecompositionUtil::getOperatorPrecendence(utf8_c) != OP_PREC::ERR) 
                isLogicalConn = true;
		}

        if (utf8_c == "(") {
            bracket_c++;
        } else if (utf8_c == ")") {
            bracket_c--;
        } else if (isLogicalConn && bracket_c == 0) {
            if (DecompositionUtil::hasHigherPrecendence(statement.mc, utf8_c)) {
                statement.mc_pos = curr_pos;
                statement.mc = utf8_c;
            }
        }
    }

    return statement.mc_pos;
}

/** Given a utf-8 string "character", if its a logical connection then it returns the connections
 *  operator precedence, else OP_ERR
 */
DecompositionUtil::OP_PREC DecompositionUtil::getOperatorPrecendence(const std::string &utfc) {
    if (utfc == "\uFFE2") return OP_PREC::NOT;
    else if (utfc == "\u2227") return OP_PREC::AND;
    else if (utfc == "\u2228") return OP_PREC::OR;
    else if (utfc == "\u2192") return OP_PREC::COND;
    else if (utfc == "\u2194") return OP_PREC::BICOND;
    else if (utfc == "\u2200") return OP_PREC::UNIVERSAL;
    else if (utfc == "\u2203") return OP_PREC::EXIST;
    else return OP_PREC::ERR;
}

/** Compares the precedence of two logical connectives and returns if the new connective precedence is 
 *  higher than the old connective precedence. 
 */
bool DecompositionUtil::hasHigherPrecendence(const std::string &oldConn, const std::string &newConn) {
    if (oldConn == "not-found") return true;

    OP_PREC oldP {DecompositionUtil::getOperatorPrecendence(oldConn)};
    OP_PREC newP {DecompositionUtil::getOperatorPrecendence(newConn)};
    
    return oldP < newP;
}

