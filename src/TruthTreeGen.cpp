#include <wx/wxprec.h>
#include <wx/gbsizer.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeGen.h"
#include "TruthTreePane.h"

std::string specialChars[9]{
    "\u2200",
    "\u2203",
    "\u2227",
    "\u2228",
    "\uFFE2",
    "\u003D",
    "\u2260",
    "\u2192",
    "\u2194"
};

void TruthTreeGen::handleTxtCtrl(wxFocusEvent &fe) 
{
    std::cout << "focus \n";
    wxObject *focusObj = fe.GetEventObject();
    if (this->frame->isTextCtrl(focusObj)) {
        std::cout << "switch\n";
        this->frame->setCurrCtrl((wxTextCtrl *) focusObj);
    }
}

bool TruthTreeGen::OnInit()
{
    this->frame = new TruthTreeFrame();
    this->frame->Show(true); 

    Bind(wxEVT_SET_FOCUS, TruthTreeGen::handleTxtCtrl, this);

    return true;
} 

IMPLEMENT_APP(TruthTreeGen);
