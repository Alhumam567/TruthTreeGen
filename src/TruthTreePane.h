#ifndef PANE_H
#define PANE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeModel.h"

class TruthTreePane : public wxScrolledWindow
{
    TruthTreeModel &model;

public:
    TruthTreePane(wxWindow* parent, wxWindowID id, TruthTreeModel &m);

    void update();
};

#endif