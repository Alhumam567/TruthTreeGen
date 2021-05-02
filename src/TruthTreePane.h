#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifndef PANE_H
#define PANE_H

class TruthTreePane : public wxScrolledWindow
{
public:
    TruthTreePane(wxWindow* parent, wxWindowID id);
};

#endif