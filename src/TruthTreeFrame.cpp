#include <wx/wxprec.h>
#include <wx/gbsizer.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeGen.h"
#include "TruthTreePane.h"

TruthTreeFrame::TruthTreeFrame() : 
    wxFrame((wxFrame *)NULL, -1,  wxT("Truth Tree Generator"), wxPoint(100,50), wxSize(650,650)),
    argCtrl {new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(350,20))},
    concCtrl {new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(200,20))},
    currCtrl {argCtrl},
    charBtns {},
    lastCursorPosition {}
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxGridBagSizer* inputSizer = new wxGridBagSizer(2,2);
    wxStaticText* argText = new wxStaticText(this, -1, "Arguments: ");
    wxStaticText* concText = new wxStaticText(this, -1, "Conclusion: ");
    inputSizer->Add(argText, wxGBPosition(0, 0));
    inputSizer->Add(argCtrl, wxGBPosition(0, 1));
    inputSizer->Add(concText, wxGBPosition(1, 0));
    inputSizer->Add(concCtrl, wxGBPosition(1, 1));
    argCtrl->SetHint("Enter your arguments");
    concCtrl->SetHint("Enter your conclusion");
    
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    
    int i = 0;
    for (auto &btn : charBtns) {
        wxString mystring(specialChars[i++].c_str(), wxConvUTF8);
        btn = new SymbolButton(this, mystring, concCtrl, currCtrl, lastCursorPosition);
    }
    for (int i = 0; i<9; i++) 
        btnSizer->Add(charBtns[i], 0, wxALL, 2);

    mainSizer->Add(btnSizer, 0, wxEXPAND);
    mainSizer->Add(inputSizer, 0, wxEXPAND);

    TruthTreePane* my_image = new TruthTreePane(this, wxID_ANY);
    mainSizer->Add(my_image, 1, wxEXPAND);

    this->SetSizer(mainSizer);
    this->Show();
}

boolean TruthTreeFrame::isTextCtrl(wxObject *obj) 
{
    return obj == argCtrl || obj == concCtrl;
}

boolean TruthTreeFrame::isSpecialCharBtn(wxObject *obj) 
{
    for (auto &x : this->charBtns) {
        if (obj == x)
            return true;
    }
    
    return false;
}

wxTextCtrl *TruthTreeFrame::getCurrCtrl() 
{
    return this->currCtrl;
}

void TruthTreeFrame::setCurrCtrl(wxTextCtrl *txt) 
{
    this->currCtrl = txt;
}

void TruthTreeFrame::updateTxtCtrlCursorPosition() 
{
    std::cout << currCtrl->GetInsertionPoint() << "\n";
    this->lastCursorPosition = currCtrl->GetInsertionPoint();
}

SymbolButton::SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt, wxTextCtrl *&cCtrl, long &cp) : 
    wxButton(frame, wxID_ANY, specialChar, wxDefaultPosition, wxSize(20,20)),
    currCtrl {cCtrl},
    lastCursorPosition {cp}
{
    Bind(wxEVT_BUTTON, SymbolButton::handleClick, this);
    std::cout << "Char: " << specialChar << " initialized\n";
}

void SymbolButton::handleClick(wxCommandEvent &ce) 
{
    wxString endString {this->currCtrl->GetRange(this->lastCursorPosition, this->currCtrl->GetLineLength(0))};

    this->currCtrl->SetValue(this->currCtrl->GetRange(0, this->lastCursorPosition).Append(this->GetLabel()).Append(endString));

    this->lastCursorPosition++;

    std::cout << "Clicked " + this->GetLabel() + "\n";
}
