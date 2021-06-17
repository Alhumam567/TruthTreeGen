#ifndef GLOBALS_H
#define GLOBALS_H

#include "TruthTreeFrame.h"

extern std::string specialChars[9];

class TruthTreeGen: public wxApp
{
    TruthTreeFrame *frame;
    bool textCtrlIsChanged;

public:
    bool OnInit();
};

#endif