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
    
    std::vector<std::string> decomp;
    std::string x {"P\u2227(Q\u2228(R\u2192(S\u2194T)))"};
    DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << "  |  ";
    std::cout << "\n";
    x = "\uFFE2P";
    bool b = DecompositionUtil::isLiteral(x);
    std::cout << b << "\n";
    x = "P";
    b = DecompositionUtil::isLiteral(x);
    std::cout << b << "\n";
    x = "\uFFE2F(abc)";
    b = DecompositionUtil::isLiteral(x);
    std::cout << b << "\n";
    x = "\uFFE2G(ij)";
    b = DecompositionUtil::isLiteral(x);
    std::cout << b << "\n";
    // x = "\uFFE2P\u2227\uFFE2Q";
    // DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << "  |  ";
    // std::cout << "\n";
    // x = "\u2200x((P\u2227Q)\u2228R)\u2227\u2203y(F(x)\u2227G(x))";
    // bool b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "P\u2228Q";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "\uFFE2P\u2228\uFFE2Q";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "\u2200x((P\u2227Q)\u2228R)\u2228\u2203y(F(x)\u2227G(x))";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "P\u2192Q";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "\uFFE2P\u2192\uFFE2Q";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << "  |  ";
    // std::cout << "\n";
    // x = "\u2200x((P\u2227Q)\u2228R)\u2192\u2203y(F(x)\u2227G(x))";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "P\u2194Q";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "\uFFE2P\u2194\uFFE2Q";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "\u2200x((P\u2227Q)\u2228R)\u2194\u2203y(F(x)\u2227G(x))";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << b;
    // std::cout << "\n";
    // x = "\uFFE2((P\u2227Q)\u2228R)";
    // b = DecompositionUtil::decompose(x, &decomp);
    // for (auto x: decomp) 
    //     std::cout << x << "  " << b << "  ";
    // std::cout << "\n";
    Bind(wxEVT_SET_FOCUS, TruthTreeGen::handleTxtCtrl, this);

    return true;
}

IMPLEMENT_APP(TruthTreeGen);
