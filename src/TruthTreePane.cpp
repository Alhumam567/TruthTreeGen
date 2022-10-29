#include "TruthTreePane.h"

TruthTreePane::TruthTreePane(wxWindow* parent, wxWindowID id, TruthTreeModel &m) : wxScrolledWindow{parent, id},  model {m}
{
    this->FitInside();
    this->SetScrollRate(5, 5);
}

void TruthTreePane::update() {
    
}