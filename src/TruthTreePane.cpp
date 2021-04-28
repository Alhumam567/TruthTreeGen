#include "TruthTreePane.h"
#include "TruthTreeGen.h"

ScrolledWidgetsPane::ScrolledWidgetsPane(wxWindow* parent, wxWindowID id) : wxScrolledWindow{parent, id}
{
    // this part makes the scrollbars show up
    this->FitInside(); // ask the sizer about the needed size
    this->SetScrollRate(5, 5);
}
