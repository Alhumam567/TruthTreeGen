#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include "TruthTreeFrame.h"

extern std::string specialChars[9];

class TruthTreeGen: public wxApp
{
    TruthTreeFrame *frame;

public:
    bool OnInit();
    void handleTxtCtrl(wxFocusEvent &fe);
};

class SymbolButton: public wxButton 
{
    wxButton *btn;
public:
    SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt);
};

#endif