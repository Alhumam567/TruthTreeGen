#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeGen.h"
#include "TruthTreePane.h"
#include "TruthTreeModel.h"
#include "StatementUtil.h"

std::string specialChars[9]{
    "\u2200", // Universal Quantifier
    "\u2203", // Existential Quantifier
    "\uFFE2", // Negation
    "\u2227", // Conjunction
    "\u2228", // Disjunction
    "\u2192", // Conditional
    "\u2194", // Biconditional
    "\u003D", // Equality
    "\u2260", // Inequality
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

    std::vector<std::string> args {"P\u2227Q"};
    std::string conc {"P"};

    TruthTreeModel *model = new TruthTreeModel{args, conc};
    model->printModel();
    delete model;

    // args = {"P\u2227Q"};
    // conc = "Q";
    
    // model = new TruthTreeModel(args, conc);
    // model->printModel();
    // delete model;

    // args = {"P\u2192Q", "P"};
    // conc = "Q";

    // model = new TruthTreeModel(args, conc);
    // model->printModel();
    // delete model;

    // args = {"P\u2192Q", "\uFFE2Q"};
    // conc = "\uFFE2P";

    // model = new TruthTreeModel(args, conc);
    // model->printModel();
    // delete model;

    // args = {"\uFFE2(P\u2194Q)"};
    // conc = "P\u2194\uFFE2Q";

    // model = new TruthTreeModel(args, conc);
    // model->printModel();
    // delete model;

    Bind(wxEVT_SET_FOCUS, TruthTreeGen::handleTxtCtrl, this);

    return true;
}

IMPLEMENT_APP(TruthTreeGen);
