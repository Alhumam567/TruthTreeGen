#ifndef TTF_H 
#define TTF_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


// class SymbolButton: public wxButton 
// {
//     wxTextCtrl *&currCtrl;
//     long &lastCursorPosition;

// public:
//     SymbolButton(wxFrame *frame, int id, wxString specialChar, wxTextCtrl *&currCtrl, long &cp);

//     void handleClick(wxCommandEvent &ce);
// };


class TruthTreeFrame: public wxFrame 
{
    wxTextCtrl *argCtrl, *concCtrl;
    wxTextCtrl *currCtrl;

    wxButton* charBtns[9];
    long lastCursorPosition;

    wxButton generateTreeBtn;

public:
    TruthTreeFrame();

    void handleTxtCtrlSwitch(wxFocusEvent &fe);
    void handleSymbolButton(wxCommandEvent &ce);
    void handleGenerateTree(wxCommandEvent &ce);
};


#endif