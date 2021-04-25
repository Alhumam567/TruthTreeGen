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

// A sample app that adds the scrolled pane to a frame to make this code runnable
class TruthTreeApp: public wxApp
{
    wxFrame *frame;
public:
    bool OnInit();
};

IMPLEMENT_APP(TruthTreeApp);

bool TruthTreeApp::OnInit()
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Scrolling Widgets"), wxPoint(50,50), wxSize(650,650));
    
    wxGridBagSizer* inputSizer = new wxGridBagSizer(2,2);
    wxTextCtrl* argTextbox = new wxTextCtrl(frame, -1, "default arguments", wxDefaultPosition, wxSize(350,20)); 
    wxStaticText* argText = new wxStaticText(frame, -1, "Arguments: ");
    wxTextCtrl* concTextbox = new wxTextCtrl(frame, -1, "default conclusion", wxDefaultPosition, wxSize(200,20)); 
    wxStaticText* concText = new wxStaticText(frame, -1, "Conclusion: ");
    inputSizer->Add(argText, wxGBPosition(0, 0));
    inputSizer->Add(argTextbox, wxGBPosition(0, 1));
    inputSizer->Add(concText, wxGBPosition(1, 0));
    inputSizer->Add(concTextbox, wxGBPosition(1, 1));
    
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* charBtns[9];
    int i = 0;
    for (auto &btn : charBtns) {
        wxString mystring(specialChars[i++].c_str(), wxConvUTF8);
        btn = new wxButton(frame, wxID_ANY, mystring, wxDefaultPosition, wxSize(20,20));
    }
    for (int i = 0; i<9; i++) 
        btnSizer->Add(charBtns[i], 0, wxALL, 2);

    mainSizer->Add(btnSizer, 0, wxEXPAND);
    mainSizer->Add(inputSizer, 0, wxEXPAND);

    ScrolledWidgetsPane* my_image = new ScrolledWidgetsPane(frame, wxID_ANY);
    mainSizer->Add(my_image, 1, wxEXPAND);
    
    frame->SetSizer(mainSizer);
    frame->Show();
    return true;
} 
