#include <iostream>
#include <codecvt>
#include <string>
#include <regex>

#include "TruthTreeGen.h"
#include "DecompositionUtil.h"

// std::regex bracketRegex = std::regex("\uFFE2?((\\([^())]+\\))|[A-Z])");

// std::regex DecompositionUtil::mainConnectiveRegex[9] {
//     std::regex("^\uFFE2(\\(.*\\))$"),  
//     std::regex("^\uFFE2(\\(.*\\))$"),  
//     std::regex("^\uFFE2?((\\([^())]+\\))|[A-Z])\u2227\uFFE2?((\\([^())]+\\))|[A-Z])$"),  // Conjunction
//     std::regex("^\uFFE2?((\\([^())]+\\))|[A-Z])\u2228\uFFE2?((\\([^())]+\\))|[A-Z])$"),  // Disjunction
//     std::regex("^\uFFE2(\\(.*\\))$"),                                                    // Negation
//     std::regex("^\uFFE2?((\\([^())]+\\))|[A-Z])\u2227\uFFE2?((\\([^())]+\\))|[A-Z])$"),
//     std::regex("^\uFFE2?((\\([^())]+\\))|[A-Z])\u2228\uFFE2?((\\([^())]+\\))|[A-Z])$"),
//     std::regex("^\uFFE2?((\\([^())]+\\))|[A-Z])\u2192\uFFE2?((\\([^())]+\\))|[A-Z])$"),  // Conditional
//     std::regex("^\uFFE2?((\\([^())]+\\))|[A-Z])\u2194\uFFE2?((\\([^())]+\\))|[A-Z])$"),  // Biconditional
// };

std::string *DecompositionUtil::decompose(std::string &statement)
{
    std::string mainConn = DecompositionUtil::findMainConnective(statement);

    if (mainConn == specialChars[0]) {

    }
    std::string *x = new std::string("1");
    return x;
}

std::size_t strlen_utf8(const std::string& str) {
	std::size_t length = 0;
	for (char c : str) {
		if ((c & 0xC0) != 0x80) {
			++length;
		}
	}
	return length;
}

std::string DecompositionUtil::findMainConnective(std::string &statement)
{
    std::string mainConnective {"not-found"};

    // // Check negation 
    // if (statement.substr(0,2) == specialChars[4].append("(") &&
    //         statement.back() == ')')
    //     return specialChars[4];
    // // Conjunction, Disjunction, Conditional, Biconditional

    // std::regex binaryConnectiveRegex {"(.+)[\u2227\u2228\u2192\u2194](.+)"};
    // std::smatch matches;

    // if (std::regex_search(statement, matches, binaryConnectiveRegex)) {

    // }

    int bracket_c {0};
    char c;
    std::string utf8_c;
    bool isUTF8;

    // Loop over all bytes in the sequence of characters, there could be
    // UTF-8 characters that take multiple bytes to represent
    for(std::size_t i = 0; i < statement.length(); i++){
        c = statement.at(i);
        utf8_c = c;
        isUTF8 = false;

        // If UTF-8 character
        if ((c & 0x80) == 0x80) {
            utf8_c = c;
            isUTF8 = true;

            c = statement.at(++i);
            while ((c & 0xC0) == 0x80) {
                utf8_c.append(std::string(1, c));
                c = statement.at(++i);
            }
            i--;
		}

        if (utf8_c == "(") {
            bracket_c++;
        } else if (utf8_c == ")") {
            bracket_c--;
        } else if (isUTF8 && bracket_c == 0) {
            if (DecompositionUtil::hasHigherPrecendence(mainConnective, utf8_c))
                mainConnective = utf8_c;
        }
        std::cout << utf8_c << " ";
    }
    std::cout << "\n";

    return mainConnective;
}

DecompositionUtil::OP_PREC DecompositionUtil::getOperatorPrecendence(std::string conn) {
    if (conn == "\uFFE2") return OP_PREC::NOT;
    else if (conn == "\u2227") return OP_PREC::AND;
    else if (conn == "\u2228") return OP_PREC::OR;
    else if (conn == "\u2192") return OP_PREC::COND;
    else if (conn == "\u2194") return OP_PREC::BICOND;
    else if (conn == "\u2200") return OP_PREC::UNIVERSALEXIST;
    else if (conn == "\u2203") return OP_PREC::UNIVERSALEXIST;
}

bool DecompositionUtil::hasHigherPrecendence(std::string oldConn, std::string newConn) {
    if (oldConn == "not-found") return true;

    OP_PREC oldP {DecompositionUtil::getOperatorPrecendence(oldConn)};
    OP_PREC newP {DecompositionUtil::getOperatorPrecendence(newConn)};
    // std::cout << "oldP:" << oldP << "\n";
    // std::cout << "newP:" << newP << "\n";
    return oldP <= newP;
}

