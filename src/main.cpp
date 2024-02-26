#include <string>
#include <cstring>
#include <string_view>
#include <iostream>
#include <vector>
#include <unordered_set>

#include "LogicStr.h"

void parselang(std::vector<LogicStr> formulae) {
    std::unordered_set<char> nameletters;
    std::unordered_set<char> variables;
    std::unordered_set<char> predicates;
    const std::string pred_letters {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    const std::string lowercase_letters {"abcdefghijklmnopqrstuvwxyz"};
    const std::string quantifiers {"∀∃"};
    bool prev_quant = false;

    for (auto &formula : formulae) {
        for (auto it = formula.begin(); it != formula.end(); it++) {
            uchar *uc = *it;
            if (uc->len != 1) {
                if (quantifiers.find(uc->val) != std::string::npos) prev_quant = true;
                else prev_quant = false;
                continue;
            }

            char ch = *(uc->val);
            if (pred_letters.find(ch) != std::string::npos)
                predicates.insert(ch);
            else if (lowercase_letters.find(ch) != std::string::npos && prev_quant && nameletters.find(ch) == nameletters.end())
                variables.insert(ch);
            else if (lowercase_letters.find(ch) != std::string::npos && variables.find(ch) == variables.end())
                nameletters.insert(ch);   
            prev_quant = false;
        }
    }

    std::cout << "name letters: " << nameletters.size() << std::endl;
    for (auto it = nameletters.begin(); it != nameletters.end(); it++) std::cout << *it << std::endl;

    std::cout << "variables: " << variables.size() << std::endl;
    for (auto it = variables.begin(); it != variables.end(); it++) std::cout << *it << std::endl;

    std::cout << "predicates: " << predicates.size() << std::endl;
    for (auto it = predicates.begin(); it != predicates.end(); it++) std::cout << *it << std::endl;
}

void iterate(char *str) {
    std::string _str = str;
    LogicStr l = LogicStr(_str);

    auto it = l.begin();
    auto end = l.end();

    std::cout << "Forward: " << std::endl;

    int ind = 0;
    while (it != end) {
        if (memcmp((*it)->val, str + ind, (*it)->len) != 0) {
            std::cout << (*it)->val << " " << str+ind << " " << (*it)->len << " invalid\n" << std::endl;
            break;
        } else {
            std::cout << (*it)->val << std::endl;
        }
        ind += (*it)->len;
        it++;
    }

    it = l.end();
    end = l.begin();

    std::cout << "Backward: " << std::endl;

    while (it != end) {
        it--;
        std::cout << (*it)->val << std::endl;
    }
}

int main (int argc, char **argv) {
    // θωερτψυιοπασδφγηςκλζχξωβνμ
    // ∀∃
    // ∧∨¬→↔
    // iterate("∀xFx");
    // iterate("∀x(∀yGy→Fx)");
    // iterate("∀x(∀yGby→Fax)");
    // iterate("P∧Q→R");
    // iterate("∀∃∧∨¬→↔");
    // iterate("abcdefgABCDEFG=");

    std::vector<LogicStr> formulae { 
        LogicStr("∀x(Fx∨Gx)"), 
        LogicStr("∀xFx∨∀yGy"), 
        LogicStr("∃x(Fx∨Gx)∨Fab"),
    };
    parselang(formulae);

    return 0;
}