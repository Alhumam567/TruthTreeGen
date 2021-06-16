#ifndef PANE_H
#define PANE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class TruthTreePane : public wxScrolledWindow
{
public:
    TruthTreePane(wxWindow* parent, wxWindowID id);
};

#endif