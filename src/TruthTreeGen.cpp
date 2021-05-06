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
    std::string x {"P\u2227Q"};
    DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << "  |  ";
    std::cout << "\n";
    x = "\uFFE2P\u2227\uFFE2Q";
    DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << "  |  ";
    std::cout << "\n";
    x = "\u2200x((P\u2227Q)\u2228R)\u2227\u2203y(F(x)\u2227G(x))";
    bool b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "P\u2228Q";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "\uFFE2P\u2228\uFFE2Q";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "\u2200x((P\u2227Q)\u2228R)\u2228\u2203y(F(x)\u2227G(x))";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "P\u2192Q";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "\uFFE2P\u2192\uFFE2Q";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << "  |  ";
    std::cout << "\n";
    x = "\u2200x((P\u2227Q)\u2228R)\u2192\u2203y(F(x)\u2227G(x))";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "P\u2194Q";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "\uFFE2P\u2194\uFFE2Q";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "\u2200x((P\u2227Q)\u2228R)\u2194\u2203y(F(x)\u2227G(x))";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << b;
    std::cout << "\n";
    x = "\uFFE2((P\u2227Q)\u2228R)";
    b = DecompositionUtil::decompose(x, &decomp);
    for (auto x: decomp) 
        std::cout << x << "  " << b << "  ";
    std::cout << "\n";
    // std::cout << "1Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2P";
    // std::cout << "1Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2(P\u2228Q)";
    // std::cout << "2Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "P\u2227Q";
    // std::cout << "3Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2P\u2228Q";
    // std::cout << "4Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "P\u2227\uFFE2Q";
    // std::cout << "5Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2P\u2227\uFFE2Q";
    // std::cout << "6Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2(P\u2192Q)";
    // std::cout << "7Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2((P\u2227Q)\u2228R)";
    // std::cout << "8Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\u2200x((P\u2227Q)\u2228R)";
    // std::cout << "9Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\u2200x((P\u2227Q)\u2228R)\u2227\u2203y(F(x)\u2227G(x))";
    // std::cout << "10Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2\u2200x((P\u2227Q)\u2228R)";
    // std::cout << "11Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\uFFE2R(ab)\u2227R(ba)\u2192a\u2260b";
    // std::cout << "12Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\u2200y\u2203z(x=f(y)\u2194x=z)";
    // std::cout << "13Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "\u2203x\u2203yx\u2260y\u2228\u2200xFx\u2192\u2200x\u2203y(Fx\u2194x=y)";
    // std::cout << "14Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    // x = "P\u2194(Q\u2192(S\u2227R))";
    // std::cout << "15Main conn: " + DecompositionUtil::findMainConnective(x) << "\n";
    Bind(wxEVT_SET_FOCUS, TruthTreeGen::handleTxtCtrl, this);

    return true;
}

IMPLEMENT_APP(TruthTreeGen);
