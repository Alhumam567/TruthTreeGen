#include <iostream>
#include <codecvt>
#include <string>
#include <regex>

#include "TruthTreeGen.h"
#include "DecompositionUtil.h"

std::size_t strlen_utf8(const std::string& str) {
	std::size_t length = 0;
	for (char c : str) {
		if ((c & 0xC0) != 0x80) {
			++length;
		}
	}
	return length;
}

bool DecompositionUtil::decompose(std::string &statement, std::vector<std::string> *decomposedStatement)
{
    bool split;

    std::string *mainConn = new std::string("");
    int pos = DecompositionUtil::findMainConnective(statement, mainConn);
    OP_PREC op = DecompositionUtil::getOperatorPrecendence(*mainConn);

    // Decomposing rules
    switch (op){
        // Universal/Existential Case
        case OP_PREC::UNIVERSAL: {
            if (*mainConn == "\u2200") // Universal
            {}
            else                      // Existential
            {}
            break;
        }
        // And: Split conjuncts
        case OP_PREC::AND: {
            decomposedStatement->resize(2);

            decomposedStatement->at(0) = statement.substr(0, pos-1);
            decomposedStatement->at(1) = statement.substr(pos+1, strlen_utf8(statement) - 1);

            split = false;
            break;
        }
        case OP_PREC::OR: {
            break;
        }
        case OP_PREC::NOT: {
            break;
        }
        case OP_PREC::COND: {
            if (*mainConn == "\u2192") // Conditional
            {}
            else                      // Biconditional
            {}
            break;
        }
        case OP_PREC::ERR: {
            break;
        }
    }

    return split;
}

int DecompositionUtil::findMainConnective(const std::string &statement, std::string *mainConnective)
{
    *mainConnective = "nuts";

    int bracket_c {0}, pos {-1};
    char c;
    std::string utf8_c;
    bool isLogicalConn;

    // Loop over all bytes in the sequence of characters, there could be
    // UTF-8 characters that take multiple bytes to represent
    for(std::size_t i = 0; i < statement.length(); i++){
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
                pos = i;
                *mainConnective = utf8_c;
            }
        }
        std::cout << utf8_c << " ";
    }
    std::cout << "\n";

    return pos;
}

DecompositionUtil::OP_PREC DecompositionUtil::getOperatorPrecendence(std::string &conn) {
    if (conn == "\uFFE2") return OP_PREC::NOT;
    else if (conn == "\u2227") return OP_PREC::AND;
    else if (conn == "\u2228") return OP_PREC::OR;
    else if (conn == "\u2192") return OP_PREC::COND;
    else if (conn == "\u2194") return OP_PREC::BICOND;
    else if (conn == "\u2200") return OP_PREC::UNIVERSAL;
    else if (conn == "\u2203") return OP_PREC::EXIST;
    else return OP_PREC::ERR;
}

bool DecompositionUtil::hasHigherPrecendence(std::string &oldConn, std::string &newConn) {
    if (oldConn == "not-found") return true;

    OP_PREC oldP {DecompositionUtil::getOperatorPrecendence(oldConn)};
    OP_PREC newP {DecompositionUtil::getOperatorPrecendence(newConn)};
    
    return oldP < newP;
}

