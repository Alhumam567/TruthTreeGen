// wxWidgets "Hello World" Program available on https://docs.wxwidgets.org/trunk/overview_helloworld.html

// For compilers that support precompilation, includes "wx/wx.h".
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

// void handleTxtCtrlFocus(wxFocusEvent &fe) {
//     std::cout << fe.GetEventObject();
// }

// A sample app that adds the scrolled pane to a frame to make this code runnable
class TruthTreeApp: public wxApp
{
public:
    bool OnInit();
};


class TruthTreeFrame: public wxFrame 
{
public:
    TruthTreeFrame();
};

class SymbolButton: public wxButton 
{
    wxButton *btn;
public:
    SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt);
};



SymbolButton::SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt) : 
    wxButton(frame, wxID_ANY, specialChar, wxDefaultPosition, wxSize(20,20))
{
    std::cout << "Char: " << specialChar << " initialized\n";
}

TruthTreeFrame::TruthTreeFrame() : 
    wxFrame((wxFrame *)NULL, -1,  wxT("Truth Tree Generator"), wxPoint(100,50), wxSize(650,650))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxGridBagSizer* inputSizer = new wxGridBagSizer(2,2);
    wxTextCtrl* argTextbox = new wxTextCtrl(this, -1, "default arguments", wxDefaultPosition, wxSize(350,20)); 
    wxStaticText* argText = new wxStaticText(this, -1, "Arguments: ");
    wxTextCtrl* concTextbox = new wxTextCtrl(this, -1, "default conclusion", wxDefaultPosition, wxSize(200,20)); 
    wxStaticText* concText = new wxStaticText(this, -1, "Conclusion: ");
    inputSizer->Add(argText, wxGBPosition(0, 0));
    inputSizer->Add(argTextbox, wxGBPosition(0, 1));
    inputSizer->Add(concText, wxGBPosition(1, 0));
    inputSizer->Add(concTextbox, wxGBPosition(1, 1));
    
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    SymbolButton* charBtns[9];
    
    int i = 0;
    for (auto &btn : charBtns) {
        wxString mystring(specialChars[i++].c_str(), wxConvUTF8);
        btn = new SymbolButton(this, mystring, concTextbox);
    }
    for (int i = 0; i<9; i++) 
        btnSizer->Add(charBtns[i], 0, wxALL, 2);

    mainSizer->Add(btnSizer, 0, wxEXPAND);
    mainSizer->Add(inputSizer, 0, wxEXPAND);

    ScrolledWidgetsPane* my_image = new ScrolledWidgetsPane(this, wxID_ANY);
    mainSizer->Add(my_image, 1, wxEXPAND);
    
    this->SetSizer(mainSizer);
    this->Show();
}

IMPLEMENT_APP(TruthTreeApp);

bool TruthTreeApp::OnInit()
{
    TruthTreeFrame *frame = new TruthTreeFrame();
    frame->Show(true); 
    return true;
} 
