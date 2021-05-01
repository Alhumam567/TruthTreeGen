#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

extern std::string specialChars[9];

class TruthTreeFrame: public wxFrame 
{
public:
    wxTextCtrl *argCtrl, *concCtrl;
    wxTextCtrl *currCtrl;

    TruthTreeFrame();
    wxTextCtrl *getCurrCtrl();
    void setCurrCtrl(wxTextCtrl *txt);
};

class TruthTreeApp: public wxApp
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