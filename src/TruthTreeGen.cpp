#include <wx/wxprec.h>
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
    wxObject *focusObj = fe.GetEventObject();
    if (this->frame->isTextCtrl(focusObj)) 
        this->frame->setCurrCtrl((wxTextCtrl *) focusObj);
    fe.Skip();
}

bool TruthTreeGen::OnInit()
{
    this->frame = new TruthTreeFrame();
    this->frame->Show(true); 

    Bind(wxEVT_SET_FOCUS, TruthTreeGen::handleTxtCtrl, this);

    return true;
} 

IMPLEMENT_APP(TruthTreeGen);
