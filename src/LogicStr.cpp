#include <algorithm>
#include <string>

#include "LogicStr.h"

LogicStr::LogicStr(const std::string str) : v (str) {
    int len = 0;
    int byte_len = strlen(str);

    int i = 0;
    while (byte_len) {
        if ((str[i] & 0x80) == 0x80) { // utf8
            while ((str[i] & 0xC0) == 0x80) {
                byte_len--;
                i++;
            }
        } else byte_len--;
        len++;
    }

    this->chars = calloc(len, sizeof(u_char));

    int byte_len = strlen(str);

    int i = 0;
    while (byte_len) {
        if ((str[i] & 0x80) == 0x80) { // utf8
            while ((str[i] & 0xC0) == 0x80) {
                byte_len--;
                i++;
            }
        } else byte_len--;
        len++;
    }

}
LogicStr::~LogicStr() {}
U_Char_Iterator *LogicStr::begin() { 
    uchar uc;
    auto s = this->v.begin();

    int len = 1;
    if ((*s & 0x80) == 0x80) { // utf8
        while ((*(++s) & 0xC0) == 0x80) len++;
    }
    uc.v = &(this->v[0]);
    uc.len = len;
    
    return U_Char_Iterator(uc);
}
U_Char_Iterator *LogicStr::end() {
    uchar 
}

// uchar *LogicStr::begin() {
//     uchar *c = (uchar *)calloc(1, sizeof(uchar));
//     auto s = this->v.begin();
    
//     int len = 1;
//     if ((*s & 0x80) == 0x80) { // utf8
//         while ((*(++s) & 0xC0) == 0x80) len++;
//     }
//     c->v = &(this->v[0]);
//     c->len = len;

//     return c;
// }

// uchar *LogicStr::end() {
//     return NULL;
// }

// uchar LogicStr::operator++(int) {
    

//     return uc;
// }

// uchar LogicStr::operator[](std::size_t idx) { return this->v[i]; }