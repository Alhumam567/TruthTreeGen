#include <wx/wxprec.h>
#include <wx/gbsizer.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeGen.h"
#include "TruthTreePane.h"
#include "TruthTreeModel.h"

TruthTreeFrame::TruthTreeFrame() : 
    wxFrame((wxFrame *)NULL, -1,  wxT("Truth Tree Generator"), wxPoint(100,50), wxSize(650,650)),
    argCtrl {new wxTextCtrl(this, 0, "", wxDefaultPosition, wxSize(350,20))},
    concCtrl {new wxTextCtrl(this, 1, "", wxDefaultPosition, wxSize(200,20))},
    currCtrl {argCtrl},
    charBtns {},
    lastCursorPosition {},
    generateTreeBtn {this, 2}
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxGridBagSizer* inputSizer = new wxGridBagSizer(2,2);
    wxStaticText* argText = new wxStaticText(this, -1, "Arguments: ");
    wxStaticText* concText = new wxStaticText(this, -1, "Conclusion: ");
    
    argCtrl->SetHint("Enter your arguments");
    concCtrl->SetHint("Enter your conclusion");
    generateTreeBtn.SetLabel("Generate Tree");

    inputSizer->Add(argText, wxGBPosition(0, 0));
    inputSizer->Add(argCtrl, wxGBPosition(0, 1));
    inputSizer->Add(concText, wxGBPosition(1, 0));
    inputSizer->Add(concCtrl, wxGBPosition(1, 1));
    inputSizer->Add(&generateTreeBtn, wxGBPosition(1, 2));
    
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    
    int i = 0;
    for (auto &btn : charBtns) {
        wxString mystring(specialChars[i++].c_str(), wxConvUTF8);
        btn = new SymbolButton(this, i + 2, mystring, currCtrl, lastCursorPosition);
        // btn->Bind(wxEVT_BUTTON, TruthTreeFrame::handleSymbolButton, this);
    }
    for (int i = 0; i<9; i++) 
        btnSizer->Add(charBtns[i], 0, wxALL, 2);

    mainSizer->Add(btnSizer, 0, wxEXPAND);
    mainSizer->Add(inputSizer, 0, wxEXPAND);

    TruthTreePane* my_image = new TruthTreePane(this, wxID_ANY);
    mainSizer->Add(my_image, 1, wxEXPAND);

    argCtrl->Bind(wxEVT_SET_FOCUS, TruthTreeFrame::handleTxtCtrlSwitch, this);
    concCtrl->Bind(wxEVT_SET_FOCUS, TruthTreeFrame::handleTxtCtrlSwitch, this);
    generateTreeBtn.Bind(wxEVT_BUTTON, TruthTreeFrame::handleGenerateTree, this);
    Bind(wxEVT_BUTTON, &TruthTreeFrame::handleSymbolButton, this, 3, 11);

    this->SetSizer(mainSizer);
    this->Show();
}

void TruthTreeFrame::handleGenerateTree(wxCommandEvent &ce)
{
    std::cout << "Arguments: " << this->argCtrl->GetValue() << "\n";
    std::cout << "Conc: " << this->concCtrl->GetValue() << "\n";

    std::string argString = std::string(this->argCtrl->GetValue().ToUTF8());
    std::vector <std::string> args;
    std::stringstream check1(argString);
      
    std::string intermediate;
      
    // Tokenizing w.r.t. space
    while(getline(check1, intermediate, ',')) args.push_back(intermediate);
    
    TruthTreeModel m {args, std::string(this->concCtrl->GetValue().ToUTF8())};
    m.printModel();
}

void TruthTreeFrame::handleTxtCtrlSwitch(wxFocusEvent &fe) {
    wxObject *focusObj = fe.GetEventObject();
    this->currCtrl = (wxTextCtrl *) focusObj;
    std::cout << "Switched to: " << focusObj << "\n";

    fe.Skip();
}

void TruthTreeFrame::handleSymbolButton(wxCommandEvent &ce) {
    wxObject *o = ce.GetEventObject();
    SymbolButton *sb = (SymbolButton *) o;

    std::cout << "Bressed: " << sb->GetLabel() << "\n";

    this->lastCursorPosition = currCtrl->GetInsertionPoint();
    this->currCtrl->SetValue(this->currCtrl->GetValue().insert(this->lastCursorPosition, sb->GetLabel()));
    
    currCtrl->SetFocus();
    currCtrl->SetInsertionPoint(++lastCursorPosition);
}


SymbolButton::SymbolButton(wxFrame *frame, int id, wxString specialChar, wxTextCtrl *&cCtrl, long &cp) : 
    wxButton(frame, id, specialChar, wxDefaultPosition, wxSize(20,20)),
    currCtrl {cCtrl},
    lastCursorPosition {cp}
{
    // this->Bind(wxEVT_BUTTON, SymbolButton::handleClick, this);
    std::cout << "Char: " << specialChar << " initialized with id: " << id << "\n";
}

void SymbolButton::handleClick(wxCommandEvent &ce) { }
