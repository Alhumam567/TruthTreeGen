#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeModel.h"
#include "TruthTreePane.h"


class TruthTreeFrame: public wxFrame 
{
    wxTextCtrl *argCtrl, *concCtrl;
    wxTextCtrl *currCtrl;

    wxButton* charBtns[9];
    long lastCursorPosition;

    TruthTreeModel model;
    TruthTreePane *treeDiag;

    wxButton generateTreeBtn;

public:
    TruthTreeFrame();

    void handleTxtCtrlSwitch(wxFocusEvent &fe);
    void handleSymbolButton(wxCommandEvent &ce);
    void handleGenerateTree(wxCommandEvent &ce);
};