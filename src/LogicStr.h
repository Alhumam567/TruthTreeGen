#include <algorithm>
#include <string>

typedef struct utf8_char {
    int len;
    const char *val;
} u_char;

class U_Char_Iterator : public std::iterator<std::forward_iterator_tag, u_char, int, const u_char *, u_char> {
    u_char ch;
    
    u_char *arr;
public:
    U_Char_Iterator(u_char start);

    operator*() { return ch; }
    operator++(); //ch++
    operator++(int); //++ch
    operator--(); //ch--
    operator--(int); //--ch
    operator==(u_char_it it2);
    operator!=(u_char_it it2);
};

class LogicStr {
private:
    u_char *chars;
public:
    const std::string v;

    LogicStr(const std::string str);
    ~LogicStr();

    U_Char_Iterator *begin();
    U_Char_Iterator *end();
};