#ifndef REGEX_H
    #include <regex>
#endif

#ifndef UTIL_H
#define UTIL_H

class DecompositionUtil
{
public:
    static std::regex mainConnectiveRegex[2];
    static std::string *decompose(std::string &statement);
    static std::string findMainConnective(std::string &statement);
};



#endif