#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

extern std::string specialChars[9];

class TruthTreeApp: public wxApp
{
public:
    bool OnInit();
};

class TruthTreeFrame: public wxFrame 
{
public:
    TruthTreeFrame();
};

class SymbolButton: public wxButton 
{
    wxButton *btn;
public:
    SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt);
};


#endif