// wxWidgets "Hello World" Program available on https://docs.wxwidgets.org/trunk/overview_helloworld.html

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#include <wx/gbsizer.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <iostream>

class ScrolledWidgetsPane : public wxScrolledWindow
{
    
public:
    ScrolledWidgetsPane(wxWindow* parent, wxWindowID id) : wxScrolledWindow(parent, id)
    {
        // this part makes the scrollbars show up
        this->FitInside(); // ask the sizer about the needed size
        this->SetScrollRate(5, 5);
    }

};

// A sample app that adds the scrolled pane to a frame to make this code runnable
class MyApp: public wxApp
{
    wxFrame *frame;
public:
    
    bool OnInit()
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
        wxButton* uq = new wxButton(frame, wxID_ANY, wxT("∀"), wxDefaultPosition, wxSize(20,20));
        wxButton* eq = new wxButton(frame, wxID_ANY, wxT("∃"), wxDefaultPosition, wxSize(20,20));
        wxButton* neg = new wxButton(frame, wxID_ANY, wxT("¬"), wxDefaultPosition, wxSize(20,20));
        wxButton* conj = new wxButton(frame, wxID_ANY, wxT("∧"), wxDefaultPosition, wxSize(20,20));
        wxButton* disj = new wxButton(frame, wxID_ANY, wxT("∨"), wxDefaultPosition, wxSize(20,20));
        wxButton* e = new wxButton(frame, wxID_ANY, wxT("="), wxDefaultPosition, wxSize(20,20));
        wxButton* ne = new wxButton(frame, wxID_ANY, wxT("≠"), wxDefaultPosition, wxSize(20,20));
        wxButton* cond = new wxButton(frame, wxID_ANY, wxT("→"), wxDefaultPosition, wxSize(20,20));
        wxButton* bicond = new wxButton(frame, wxID_ANY, wxT("↔"), wxDefaultPosition, wxSize(20,20));
        btnSizer->Add(uq, 0, wxALL, 2);
        btnSizer->Add(eq, 0, wxALL, 2);
        btnSizer->Add(neg, 0, wxALL, 2);
        btnSizer->Add(conj, 0, wxALL, 2);
        btnSizer->Add(disj, 0, wxALL, 2);
        btnSizer->Add(e, 0, wxALL, 2);
        btnSizer->Add(ne, 0, wxALL, 2);
        btnSizer->Add(cond, 0, wxALL, 2);
        btnSizer->Add(bicond, 0, wxALL, 2);

        mainSizer->Add(btnSizer, 0, wxEXPAND);
        mainSizer->Add(inputSizer, 0, wxEXPAND);
    
        ScrolledWidgetsPane* my_image = new ScrolledWidgetsPane(frame, wxID_ANY);
        mainSizer->Add(my_image, 1, wxEXPAND);
        
        frame->SetSizer(mainSizer);
        frame->Show();
        return true;
    } 
};

IMPLEMENT_APP(MyApp);
