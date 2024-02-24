#include <algorithm>
#include <string>

typedef struct utf8_char {
    int len;
    char *val;
} uchar;

class UChar_Iterator : public std::iterator<std::forward_iterator_tag, uchar *, int, const uchar **, char *> {
public:
    uchar **chs;
    size_t chs_sz;
    int ind;

    explicit UChar_Iterator(uchar **arr, size_t sz, int start);

    reference operator*() { return chs[ind]->val; }
    UChar_Iterator operator++() { auto ret = *this; ind++; return ret; } //ch++
    UChar_Iterator operator++(int) { ind++; return *this; } //++ch
    UChar_Iterator operator--() { auto ret = *this; ind--; return ret; } //ch--
    UChar_Iterator operator--(int) { ind--; return *this; } //--ch
    bool operator==(UChar_Iterator it2) { return ind == it2.ind && chs == it2.chs; }
    bool operator!=(UChar_Iterator it2) { return ind != it2.ind || chs != it2.chs; }
};

class LogicStr {
private:
    uchar **chars;
    size_t chars_sz;
public:
    const std::string v;

    LogicStr(const std::string str);
    ~LogicStr();

    UChar_Iterator begin();
    UChar_Iterator end();
};