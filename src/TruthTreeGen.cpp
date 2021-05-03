#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeGen.h"
#include "TruthTreePane.h"
#include "DecompositionUtil.h"

std::string specialChars[9]{
    "\u2200", // Universal Quantifier
    "\u2203", // Existential Quantifier
    "\u2227", // Conjunction
    "\u2228", // Disjunction
    "\uFFE2", // Negation
    "\u003D", // Equality
    "\u2260", // Inequality
    "\u2192", // Conditional
    "\u2194"  // Biconditional
};

void TruthTreeGen::handleTxtCtrl(wxFocusEvent &fe) 
{
    wxObject *focusObj = fe.GetEventObject();

    if (this->frame->isTextCtrl(focusObj)) {
        this->textCtrlIsChanged = true;
        this->frame->setCurrCtrl((wxTextCtrl *) focusObj);
    }
    else if (this->textCtrlIsChanged && this->frame->isSpecialCharBtn(focusObj)) {
        this->textCtrlIsChanged = false;
        this->frame->updateTxtCtrlCursorPosition();
    }
    
    fe.Skip();
}

bool TruthTreeGen::OnInit()
{
    this->frame = new TruthTreeFrame();
    this->frame->Show(true); 
    this->textCtrlIsChanged = true;
    std::string x {"\uFFE2(PQ)"};
    std::cout << "Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    Bind(wxEVT_SET_FOCUS, TruthTreeGen::handleTxtCtrl, this);

    return true;
}

IMPLEMENT_APP(TruthTreeGen);
