#include <iostream>
#include <codecvt>
#include <string>
#include <regex>

#include "Statement.h"
#include "TruthTreeGen.h"
#include "DecompositionUtil.h"

std::size_t DecompositionUtil::strlen_utf8(const std::string& str) {
	std::size_t length = 0;
	for (char c : str) {
		if ((c & 0xC0) != 0x80) {
			++length;
		}
	}
	return length;
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
bool DecompositionUtil::decompose(const std::string &statement, std::vector<std::string> *decomposedStatement)
{
    bool split;

    std::string *mainConn = new std::string();
    int pos = DecompositionUtil::findMainConnective(statement, mainConn);
    OP_PREC op = DecompositionUtil::getOperatorPrecendence(*mainConn);

    std::string left {statement.substr(0, pos)};
    std::string right {statement.substr(pos+3, statement.length() - (pos+3))}; // start at pos+3 bc main connectives are 3 bytes long

    // Invalid statement cases:
    //      - Left is empty string even though mainconnective is not negation
    //      - Right is empty string
    if ((left.length() == 0 && *mainConn != "\uFFE2") || right.length() == 0) {
        decomposedStatement->resize(1);
        decomposedStatement->at(0) = "error, invalid statement";
        return false;
    }

    // Strip any unnecessary brackets
    std::string *conn = new std::string();
    DecompositionUtil::findMainConnective(left, conn);
    if (left.length() > 0 && *conn == "nuts" && left.at(0) == '(')
        left = left.substr(1, left.length()-2);
    
    DecompositionUtil::findMainConnective(right, conn);
    if (*conn == "nuts" && right.at(0) == '(') {
        right = right.substr(1, right.length()-2);
    }
    delete conn;

    bool decompose {true}, fNeg {false};

    // Dum dum finite state automata
    while (decompose) {
        // Decomposing rules
        switch (op) {
            // Universal/Existential Case
            case OP_PREC::UNIVERSAL: {
                if (*mainConn == "\u2200") // Universal
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

                split = decompose = false;
                break;
            }
            // Or: Split disjuncts into different branches
            case OP_PREC::OR: {
                decomposedStatement->resize(2);

                decomposedStatement->at(0) = left;
                decomposedStatement->at(1) = right;

                split = true;
                decompose = false;
                break;
            }
            // Not: Depends on inner nested connective
            case OP_PREC::NOT: {
                // Check its not a literal
                if (!DecompositionUtil::isLiteral(right)) {
                    pos = DecompositionUtil::findMainConnective(right, mainConn);
                    op = DecompositionUtil::getOperatorPrecendence(*mainConn);
                    
                    left = right.substr(0, pos);
                    right = right.substr(pos+3, right.length() - (pos+3)); // start at pos+3 bc main connectives are 3 bytes long
                    
                    decompose = true;
                    fNeg = true;
                } else {}

                break;
            }
            // Conditional/Biconditional case
            case OP_PREC::COND: {
                if (*mainConn == "\u2192") { // Conditional
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
                decompose = false;
                break;
            }
            case OP_PREC::ERR: {
                decompose = false;
                break;
            }
        }
    }

    // Cover negation:
    if (fNeg) {
        // Biconditional special case
        if (*mainConn == "\u2194") { 
            split = true;
        
            // decomposedStatement->at(1) = std::string("\uFFE2").append(decomposedStatement->at(1)); // Left true Right false
            addNegation(decomposedStatement->at(1));
            // decomposedStatement->at(2) = std::string("\uFFE2").append(decomposedStatement->at(2)); // Left false Right true
            addNegation(decomposedStatement->at(3));
        } else {
            split = !split;

            for (auto &ds : *decomposedStatement) {
                // ds = std::string("\uFFE2").append(ds);
                addNegation(ds);
            }
        }   
    }

    delete mainConn;
    return split;
}

/** Computes if statement is a literal by first checking if the main connective is either
 *  negation or none, if its none then statement is guaranteed to be literal, if negation
 *  check next character following negation is not an opening bracket.
 */
bool DecompositionUtil::isLiteral(const std::string &statement) 
{
    std::string *mainConn = new std::string();
    DecompositionUtil::findMainConnective(statement, mainConn);

    // Main connective is neither a negation or literal
    if (*mainConn != "\uFFE2" && *mainConn != "nuts") { 
        delete mainConn;
        return false; 
    }

    if (*mainConn == "\uFFE2") {
        std::string innerStatement = statement.substr(3, statement.length()-3); // Ignore negation

        if (innerStatement[0] == '(') {
            delete mainConn;
            return false;
        }
    }
    
    delete mainConn;
    return true;
}

/** Assumes inputs are literals, simply checks if they are negations of each other */
bool DecompositionUtil::isNegations(const std::string &literal1, const std::string &literal2) {
    std::string *mc1 = new std::string(), *mc2 = new std::string();
    DecompositionUtil::findMainConnective(literal1, mc1);
    DecompositionUtil::findMainConnective(literal2, mc2);

    if (*mc1 == "\uFFE2") return literal1.substr(3, literal1.length()) == literal2;
    else if (*mc2 == "\uFFE2") return literal2.substr(3, literal2.length()) == literal1;
    else return false;
}

/** This function adds negations to any kind of statement. If a double negation is be created
 *  then the statement will be automatically decomposed to remove the negation. 
 */
void DecompositionUtil::addNegation(std::string &statement) {
    if (!DecompositionUtil::isLiteral(statement)) { // If compound
        std::string *mc = new std::string();
        DecompositionUtil::findMainConnective(statement, mc);
        if (*mc == "\uFFE2")
            statement = statement.substr(4,statement.length() - 4 - 1); // Skip neg and opening bracket, don't include outer bracket
        else 
            statement = "\uFFE2(" + statement + ")";
    } else {                                        // If literal
        if (statement.substr(0, 3) == "\uFFE2")     // Check for existing neg
            statement = statement.substr(3, statement.length() - 3);
        else {
            statement = "\uFFE2" + statement;
        }
    }
}

int DecompositionUtil::findMainConnective(const std::string &statement, std::string *mainConnective)
{
    *mainConnective = "nuts";

    int bracket_c {0}, pos {-1}, curr_pos {-1};
    char c;
    std::string utf8_c;
    bool isLogicalConn;

    // Loop over all bytes in the sequence of characters, there could be
    // UTF-8 characters that are variably lengthed
    for(std::size_t i = 0; i < statement.length(); i++){
        curr_pos = i;
        c = statement.at(i);
        utf8_c = c;
        isLogicalConn = false;

        // If UTF-8 character
        if ((c & 0x80) == 0x80) {
            utf8_c = c;

            c = statement.at(++i);
            while ((c & 0xC0) == 0x80) {
                utf8_c.append(std::string(1, c));
                c = statement.at(++i);
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
            if (DecompositionUtil::hasHigherPrecendence(*mainConnective, utf8_c)) {
                pos = curr_pos;
                *mainConnective = utf8_c;
            }
        }
    }

    return pos;
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

