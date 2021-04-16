// wxWidgets "Hello World" Program available on https://docs.wxwidgets.org/trunk/overview_helloworld.html

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
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
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Scrolling Widgets"), wxPoint(50,50), wxSize(650,650));
        
        ScrolledWidgetsPane* my_image = new ScrolledWidgetsPane(frame, wxID_ANY);
        sizer->Add(my_image, 1, wxEXPAND);
        frame->SetSizer(sizer);
        
        frame->Show();
        return true;
    } 
};

IMPLEMENT_APP(MyApp);

