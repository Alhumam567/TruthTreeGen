#include <string>
#include <cstring>
#include <string_view>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "LogicStr.h"

const std::string pred_letters {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const std::string lowercase_letters {"abcdefghijklmnopqrstuvwxyz"};
const std::string quantifiers {"∀∃"};
const std::string tp_functors {"∧∨→↔"};
const std::string negation {"¬"};
const size_t npos = std::string::npos;

void parselang(std::vector<LogicStr> formulae) {
    std::unordered_set<char> nameletters;
    std::unordered_set<char> variables;
    std::unordered_map<char,std::unordered_set<int>> predicates;
    bool is_decision_procedure = true;

    bool prev_quant = false;

    for (auto &formula : formulae) {
        for (auto it = formula.begin(); it != formula.end(); it++) {
            uchar *uc = *it;
            if (uc->len != 1) {
                if (quantifiers.find(uc->val) != npos) {
                    prev_quant = true;
                    is_decision_procedure = false;
                }
                else prev_quant = false;
                continue;
            }

            char ch = *(uc->val);
            if (pred_letters.find(ch) != npos){
                int places = 0;
                auto it2 = it;
                it2++;
                while (it2 != formula.end() && lowercase_letters.find(*((*it2)->val)) != npos) { 
                    it2++;
                    places++;
                }

                if (predicates.find(ch) != predicates.end()) 
                    predicates[ch].insert(places);
                else
                    predicates.insert({ch,std::unordered_set<int>{places}});
            }
            else if (lowercase_letters.find(ch) != npos && prev_quant && nameletters.find(ch) == nameletters.end())
                variables.insert(ch);
            else if (lowercase_letters.find(ch) != npos && variables.find(ch) == variables.end())
                nameletters.insert(ch);   
            prev_quant = false;
        }
    }

    std::cout << "name letters: " << nameletters.size() << std::endl;
    for (auto it = nameletters.begin(); it != nameletters.end(); it++) std::cout << *it << std::endl;

    std::cout << "variables: " << variables.size() << std::endl;
    for (auto it = variables.begin(); it != variables.end(); it++) std::cout << *it << std::endl;

    std::cout << "predicates: " << predicates.size() << std::endl;
    for (auto it = predicates.begin(); it != predicates.end(); it++) {
        std::cout << it->first << " ";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) 
            std::cout << *it2 << " ";
        std::cout << std::endl;
    } 

    std::cout << "decision procedure: " << is_decision_procedure << std::endl;
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
    // P
    // Fa
    // P∧Q
    // P→Q
    // ∀xFx

    // std::vector<LogicStr> formulae { 
    //     LogicStr("∀x(Fx∨Gx)"), 
    //     LogicStr("∀xFx∨∀yGy"), 
    //     LogicStr("∃x(Fx∨Gx)∨Fab"),
    // };
    // parselang(formulae);
    // formulae = std::vector<LogicStr>({ 
    //     LogicStr("P∧Q→R"), 
    //     LogicStr("P∧Q") 
    // });
    // parselang(formulae);

    // ∧∨¬→↔
    std::cout << "Valid Formulae Test" << std::endl;
    std::vector<LogicStr> valid_formulae = {
        LogicStr("P"),
        LogicStr("¬P"),
        LogicStr("P∧Q"), 
        LogicStr("(Q→¬R)∧¬Q"), 
        LogicStr("P∧¬Q"), 
        LogicStr("¬(P∧¬Q)"), 
        LogicStr("(P∧Q)∧R"), 
        LogicStr("R∧(P∧Q)"), 
        LogicStr("¬((Q→¬R)∧¬Q)"),
        LogicStr("∀xFx"),
        LogicStr("∀x(Fx∧Gx)"),
        LogicStr("∀x(Fx∧Gx)∧P"),
    };
    for (int i = 0; i < valid_formulae.size(); i++) 
        std::cout << verify(valid_formulae[i].begin(), valid_formulae[i].end()) << std::endl;

    std::cout << "Invalid Formulae Test" << std::endl;
    std::vector<LogicStr> invalid_formulae = {
        LogicStr("→RR"), 
        LogicStr("→"), 
        LogicStr("¬"), 
        LogicStr("¬P∧"), 
        LogicStr("P∧¬→R"),
        LogicStr("()"), 
        LogicStr(")"), 
        LogicStr("¬((Q→¬R)P∧¬Q"),
        LogicStr("¬(Q→¬R)∧¬Q)"),
        LogicStr("∀x"),
        LogicStr("∀x∧(Fx∧Gx)"),
    };
    for (int i = 0; i < invalid_formulae.size(); i++) 
        std::cout << verify(invalid_formulae[i].begin(), invalid_formulae[i].end()) << std::endl;
    
    return 0;
}