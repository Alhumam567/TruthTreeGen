#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeGen.h"
#include "TruthTreePane.h"
#include "TruthTreeModel.h"

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


bool TruthTreeGen::OnInit()
{
    this->frame = new TruthTreeFrame();
    this->frame->Show(true); 
    this->textCtrlIsChanged = true;

    std::vector<std::string> args {"P\u2227Q"};
    std::string conc {"P"};

    TruthTreeModel *model = new TruthTreeModel{args, conc};
    // model->printModel();
    delete model;

    args = {"\uFFE2(P\u2194Q)"};
    conc = "P\u2194\uFFE2Q";

    model = new TruthTreeModel(args, conc);
    // model->printModel();
    delete model;

    // Bind(wxEVT_SET_FOCUS, TruthTreeFrame::handleTxtCtrl, this->frame);

    return true;
}

IMPLEMENT_APP(TruthTreeGen);
