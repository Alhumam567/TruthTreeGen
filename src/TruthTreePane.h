#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifndef PANE_H
#define PANE_H

class ScrolledWidgetsPane : public wxScrolledWindow
{
public:
    ScrolledWidgetsPane(wxWindow* parent, wxWindowID id);
};

#endif