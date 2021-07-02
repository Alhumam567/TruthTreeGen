#include "TruthTreePane.h"
#include "TruthTreeGen.h"

TruthTreePane::TruthTreePane(wxWindow* parent, wxWindowID id, TruthTreeModel &m) : wxScrolledWindow{parent, id},  model {m}
{
    // this part makes the scrollbars show up
    this->FitInside(); // ask the sizer about the needed size
    this->SetScrollRate(5, 5);
}
