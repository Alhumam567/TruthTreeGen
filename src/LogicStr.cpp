#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>

#include "LogicStr.h"

UChar_Iterator::UChar_Iterator(uchar **arr, size_t sz, int start) : 
    chs (arr), chs_sz (sz), ind (start) 
{ }

LogicStr::LogicStr(const std::string str) : v (str) {
    int len = 0;
    int byte_len = (int) str.length();
    
    int i = 0;
    // Calculate total number of utf8 + ascii characters 1000 0000 1100 0000
    while (byte_len) {
        if ((str[i] & 0x80) == 0x80) { // utf8
            byte_len--;
            while ((str[++i] & 0xC0) == 0x80) byte_len--;
        } else { 
            byte_len--;
            i++;
        }
        len++;
    }
    
    this->chars = (uchar **) calloc(len, sizeof(uchar *));
    this->chars_sz = len;

    i = 0;
    int si = 0;
    // Store utf8 and ascii characters as uchar arr
    while (i < this->chars_sz) {
        this->chars[i] = (uchar *) calloc(1, sizeof(uchar));
        this->chars[i]->len = 0;

        int i2 = si;
        // Calculate byte length of next uchar
        if ((str[i2] & 0x80) == 0x80) { // utf8
            i2++;
            while ((str[i2++] & 0xC0) == 0x80) { this->chars[i]->len++; }
            this->chars[i]->len++;
        } else { 
            this->chars[i]->len = 1;
        }
        this->chars[i]->val = (char *) calloc(this->chars[i]->len + 1, sizeof(char));
        
        // Store uchar
        if ((str[si] & 0x80) == 0x80) { // utf8
            int vi = 0;
            this->chars[i]->val[vi++] = str[si];
            si++;
            while ((str[si] & 0xC0) == 0x80) { 
                this->chars[i]->val[vi++] = str[si];
                si++;
            }
        } else {
            this->chars[i]->val[0] = str[si];
            si++;
        }
        this->chars[i]->val[this->chars[i]->len] = 0;
        i++;
    }
}
LogicStr::~LogicStr() {
    // for (int i = 0; i < this->chars_sz; i++) free(chars[i]);

    // free(chars);
}

UChar_Iterator LogicStr::begin() { 
    return UChar_Iterator(this->chars, this->chars_sz, 0);
}
UChar_Iterator LogicStr::end() {
    return UChar_Iterator(this->chars, this->chars_sz, this->chars_sz);
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