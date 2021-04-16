// wxWidgets "Hello World" Program available on https://docs.wxwidgets.org/trunk/overview_helloworld.html

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <iostream>

class ScrolledWidgetsPane : public wxScrolledWindow
{
    
public:
    ScrolledWidgetsPane(wxWindow* parent, wxWindowID id) : wxScrolledWindow(parent, id)
    {
        // the sizer will take care of determining the needed scroll size
        // (if you don't use sizers you will need to manually set the viewport size)
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        
        // add a series of widgets
        for (int w=1; w<=10; w++)
        {
            wxButton* b = new wxButton(this, wxID_ANY, wxString::Format(wxT("Button %i"), w));
            sizer->Add(b, 0, wxALL, 3);
        }
        
        
        this->SetSizer(sizer);

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
        
        wxGridSizer* inputSizer = new wxGridSizer(2);
        wxTextCtrl* argTextbox = new wxTextCtrl(frame, -1, "default arguments"); 
        wxStaticText* argText = new wxStaticText(frame, -1, "Arguments: ");
        wxTextCtrl* concTextbox = new wxTextCtrl(frame, -1, "default conclusion"); 
        wxStaticText* concText = new wxStaticText(frame, -1, "Conclusion: ");
        inputSizer->Add(argText);
        inputSizer->Add(argTextbox);
        inputSizer->Add(concText);
        inputSizer->Add(concTextbox);
        mainSizer->Add(inputSizer);

        ScrolledWidgetsPane* my_image = new ScrolledWidgetsPane(frame, wxID_ANY);
        mainSizer->Add(my_image, 1, wxEXPAND);
        
        frame->SetSizer(mainSizer);
        frame->Show();
        return true;
    } 
};

IMPLEMENT_APP(MyApp);
