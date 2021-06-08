#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeGen.h"
#include "TruthTreePane.h"
#include "TruthTreeModel.h"
#include "DecompositionUtil.h"

std::string specialChars[9]{
    "\u2200", // Universal Quantifier
    "\u2203", // Existential Quantifier
    "\uFFE2", // Negation
    "\u2227", // Conjunction
    "\u2228", // Disjunction
    "\u2192", // Conditional
    "\u2194",  // Biconditional
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

    std::vector<std::string> args {"\uFFE2P\u2227\uFFE2Q"};
    std::string conc {"\uFFE2(P\u2228Q)"};

    TruthTreeModel model {args, conc};
    model.printModel();

    // for (auto x : specialChars) {
    //     std::cout << "x: " << x << " , len: " << x.length() << "\n";
    // }
    // std::cout << "-------------" << "\n";
    
    // std::vector<std::string> decomp;
    // bool s;
    // std::string x;

    // x = "\uFFE2P\u2192(Q\u2228(R\u2192S))";
    // std::cout << "x: " << x << "\n";
    // s = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) {
    //     std::cout << "Len: " << x.length() << ", " << x << "  |  ";
    // }
    // std::cout << "branch: " << s << "\n";
    // std::cout << "-------------\n";

    // x = "\u2200x((P\u2227Q)\u2228R)\u2227\u2203y(F(x)\u2227G(x))";
    // std::cout << "x: " << x << "\n";
    // s = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) {
    //     std::cout << "Len: " << x.length() << ", " << x << "  |  ";
    // }
    // std::cout << "branch: " << s << "\n";
    // std::cout << "-------------\n";

    // x = "\uFFE2P\u2228\uFFE2Q";
    // std::cout << "x: " << x << "\n";
    // s = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) {
    //     std::cout << "Len: " << x.length() << ", " << x << "  |  ";
    // }
    // std::cout << "branch: " << s << "\n";
    // std::cout << "-------------\n";

    // x = "P\u2192Q";
    // std::cout << "x: " << x << "\n";
    // s = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) {
    //     std::cout << "Len: " << x.length() << ", " << x << "  |  ";
    // }
    // std::cout << "branch: " << s << "\n";
    // std::cout << "-------------\n";

    // x = "\uFFE2((P\u2227Q)\u2228R)";
    // std::cout << "x: " << x << "\n";
    // s = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) {
    //     std::cout << "Len: " << x.length() << ", " << x << "  |  ";
    // }
    // std::cout << "branch: " << s << "\n";
    // std::cout << "-------------\n";
    // std::vector<std::string> s {};
    // std::cout << s.size() << " " << s.capacity() << "\n";

    // for (auto sx : s) std::cout << sx << "\n";
    // std::cout << "======\n";
    // s.push_back("penis");
    // std::cout << s.size() << " " << s.capacity() << "\n";

    // for (auto sx : s) std::cout << sx << "\n";
    // std::cout << "======\n";

    Bind(wxEVT_SET_FOCUS, TruthTreeGen::handleTxtCtrl, this);

    return true;
}

IMPLEMENT_APP(TruthTreeGen);
