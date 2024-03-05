#include <iostream>
#include <codecvt>
#include <string>
#include <regex>
#include <unordered_map>

#include "grammar.h"
#include "StatementUtil.h"

enum class OP {
    UNQ = 0,
    EXQ = 0,
    NEG = 1,
    CONJ = 2,
    DISJ =3,
    COND = 4,
    BICOND = 4,
    EQ,
    INEQ,
    NONE = -1
};

std::unordered_map<std::string, OP> op_mapping = {
    {UNIQ, 0}, // Universal Quantifier
    {EXIQ, 0}, // Existential Quantifier
    {NEGA, 1}, // Negation
    {CONJU, 2}, // Conjunction
    {DISJU, 3}, // Disjunction
    {CONDI, 4}, // Conditional
    {BICONDI, 4} // Biconditional
};

std::size_t StatementUtil::strlen_utf8(const std::string& str) {
	std::size_t length = 0;
	for (char c : str) {
		if ((c & 0xC0) != 0x80) {
			++length;
		}
	}
	return length;
}

Statement StatementUtil::initializeStatement(const std::string &str, bool verbose) {
    Statement s {str};

    StatementUtil::findMainConnective(s);
    StatementUtil::isLiteral(s);

    if (verbose) {
        std::cout << "Initializing statement: " << str << "\n";
        std::cout << "\t mc: " << s.mc << "\n";
        std::cout << "\t mc_pos: " << s.mc_pos << "\n";
        std::cout << "\t isLiteral: " << s.isLiteral << "\n";
    }

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
bool StatementUtil::decompose(const Statement &statement, std::vector<Statement> *decomposedStatement)
{
    bool split;
    int8_t op = StatementUtil::getOperatorPrecendence(statement.mc);

    Statement left {StatementUtil::initializeStatement(statement.value.substr(0, statement.mc_pos), true)};
    Statement right {StatementUtil::initializeStatement(statement.value.substr(statement.mc_pos+3, 
                                                                                    statement.value.length() - (statement.mc_pos+3)), true)}; // start at pos+3 bc main connectives are 3 bytes long

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
    if (!noB) left = StatementUtil::initializeStatement(left.value.substr(1, left.value.length()-2),
                                                            true);
    b = 0; noB = right.value[0] != '(';
    for (int i {0}; i < right.value.length(); i++) {
        if (right.value[i] == '(') b++;
        else if (right.value[i] == ')') b--;
        if (b == 0 && i < right.value.length() - 1) { noB = true; break; }
    }
    if (!noB) right = StatementUtil::initializeStatement(right.value.substr(1, right.value.length()-2), 
                                                             true);

    // Decomposing rules
    switch (op) {
        // Universal/Existential Case
        case OP::UNQ: {
            if (statement.mc == "\u2200") // Universal
            {}
            else                      // Existential
            {}
            break;
        }
        // And: Split conjuncts in same branch
        case OP::CONJ: {
            decomposedStatement->resize(2);

            decomposedStatement->at(0) = left;
            decomposedStatement->at(1) = right;

            split = false;
            break;
        }
        // Or: Split disjuncts into different branches
        case OP::DISJ: {
            decomposedStatement->resize(2);

            decomposedStatement->at(0) = left;
            decomposedStatement->at(1) = right;

            split = true;
            break;
        }
        // Not: Depends on inner nested connective
        case OP::NEG: {
            split = StatementUtil::decompose(right, decomposedStatement);

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
        case OP::COND: {
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
    }

    return split;
}

/** Computes if statement is a literal by first checking if the main connective is either
 *  negation or none, if its none then statement is guaranteed to be literal, if negation
 *  check next character following negation is not an opening bracket.
 */
bool StatementUtil::isLiteral(Statement &statement) 
{
    // Main connective is neither a negation or literal
    if (!(statement.mc == "\uFFE2" || statement.mc == "")) { 
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

/** Assumes inputs are literals, simply checks if they are negations of each other */
bool StatementUtil::isNegations(const std::string &literal1, const std::string &literal2) {
    if (literal1.substr(0, 3) == "\uFFE2") return literal1.substr(3, literal1.length()) == literal2;
    else if (literal2.substr(0, 3) == "\uFFE2") return literal2.substr(3, literal2.length()) == literal1;
    else return false;
}

/** This function adds negations to any kind of statement. If a double negation is be created
 *  then the statement will be automatically decomposed to remove the negation. 
 */
void StatementUtil::addNegation(Statement &statement) {
    if (!statement.isLiteral) { // If compound
        if (statement.mc == "\uFFE2")
            statement = StatementUtil::initializeStatement(statement.value.substr(4, statement.value.length() - 4 - 1),
                                                                                      true); // Skip neg and opening bracket, don't include outer bracket
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

int StatementUtil::findMainConnective(Statement &statement)
{
    int bracket_c {0};
    char c;
    std::string utf8_c;
    bool isLogicalConn;

    // Loop over all bytes in the sequence of characters, there could be
    // UTF-8 characters that are variably lengthed
    for(std::size_t i = 0; i < statement.value.length(); i++){
        c = statement.value[i];
        utf8_c = c;
        isLogicalConn = false;

        // If UTF-8 character
        if ((c & 0x80) == 0x80) {
            c = statement.value[++i];
            while ((c & 0xC0) == 0x80) {
                utf8_c += c;
                c = statement.value[++i];
            }
            i--;

            if (StatementUtil::getOperatorPrecendence(utf8_c) != OP::NONE) 
                isLogicalConn = true;
		}

        if (utf8_c == "(") {
            bracket_c++;
        } else if (utf8_c == ")") {
            bracket_c--;
        } else if (isLogicalConn && bracket_c == 0) {
            if (StatementUtil::hasHigherPrecendence(statement.mc, utf8_c)) {
                statement.mc_pos = i;
                statement.mc = utf8_c;
            }
        }
    }

    return statement.mc_pos;
}

/** Given a utf-8 string "character", if its a logical connection then it returns the connections
 *  operator precedence, else OP_ERR
 */
OP StatementUtil::getOperatorPrecendence(const std::string &utfc) {
    OP op = op_mapping[utfc];
    if (op == OP::NONE) return OP::NONE;
    return op;
}

/** Compares the precedence of two logical connectives and returns if the new connective precedence is 
 *  higher than the old connective precedence. 
 */
bool StatementUtil::hasHigherPrecendence(const std::string &oldConn, const std::string &newConn) {
    if (oldConn == "not-found") return true;

    OP oldP {StatementUtil::getOperatorPrecendence(oldConn)};
    OP newP {StatementUtil::getOperatorPrecendence(newConn)};
    
    return oldP < newP;
}

const std::string pred_letters {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const std::string lowercase_letters {"abcdefghijklmnopqrstuvwxyz"};
const std::string quantifiers {"∀∃"};
const std::string tp_functors {"∧∨→↔"};
const std::string negation {"¬"};
const size_t npos = std::string::npos;

bool verify(UChar_Iterator it, UChar_Iterator end) {   
    bool prev_is_functor = true;
    
    for (; it != end; it++) {
        uchar *uc = *it;

        if (quantifiers.find(uc->val) != npos) {
            if (!prev_is_functor) return false;
            it++;
            
            if (it == end || lowercase_letters.find((*it)->val) == npos) return false;
            it++;

            if (it == end) return false;
            uc = *it;
        }

        // Opening Parenthese
        if (!strncmp(uc->val, "(", 2)) { 
            if (!prev_is_functor) return false;

            UChar_Iterator it2 = it;
            it2++;
            UChar_Iterator end2 = it2;
            int st = 0;
            while (end2 != end) {
                uc = *end2;
                if (st == 0 && !strncmp(uc->val, ")", 2)) break;
                else if (!strncmp(uc->val, "(", 2)) st++;
                else if (!strncmp(uc->val, ")", 2)) st--;
                end2++;
            }

            // Recurse into parentheses
            if (end2 == end || !verify(it2, end2)) return false;
            it = end2;
        } 
        // Negation functor 
        else if (uc->val == negation) { 
            if (!prev_is_functor) return false;
            prev_is_functor = !prev_is_functor; // keep flag as previous functor
        } 
        // Predicate letter
        else if (pred_letters.find(uc->val) != npos) {
            if (!prev_is_functor) return false;
            it++;

            // Move past predicate places
            while (it != end && lowercase_letters.find((*it)->val) != npos) it++;
            it--;
        } 
        // Fail cases
        else if (!strncmp(uc->val, ")", 2) || // Closing Parenthese
                 tp_functors.find(uc->val) != npos && prev_is_functor || // Two-place functor following functor
                 lowercase_letters.find(uc->val) != npos) { // Lowercase letter 
            return false;
        }
        
        prev_is_functor = !prev_is_functor;
    }

    return !prev_is_functor;
}

