#include <iostream>
#include <codecvt>
#include <string>
#include <regex>

#include "TruthTreeGen.h"
#include "DecompositionUtil.h"

std::regex DecompositionUtil::mainConnectiveRegex[2] {
    std::regex("^\uFFE2(\\(.*\\))$"),
    std::regex("^\uFFE2?((\\([^())]+\\))|[A-Z])\u2227\uFFE2?((\\([^())]+\\))|[A-Z])$"),
};

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
    // std::size_t len = strlen_utf8(statement);
    // std::cout << "Character length: " << len << '\n';
    std::string mainConnective {"not-found"};
    int i = 0;

    for (auto &regex : DecompositionUtil::mainConnectiveRegex){
        if (std::regex_match(statement, regex)) {
            mainConnective = specialChars[i];
            break;
        }
        i++;
    }


    // Loop over all bytes in the sequence of characters, there could be
    // UTF-8 characters that take multiple bytes to represent
    for(std::size_t i = 0; i < statement.length(); i++){
        char c = statement.at(i);

        // UTF-8 character
        if ((c & 0x80) == 0x80) {
            std::string utf8_c {c};

            c = statement.at(++i);
            while ((c & 0x80) == 0x80) {
                utf8_c.append(std::string(1, c));
                c = statement.at(++i);
            }
            i--;
            
            std::cout << utf8_c << std::endl;
		}
        // Simple ASCII character
        else {
            std::cout << c << std::endl;
        }
    }
    return mainConnective;
}