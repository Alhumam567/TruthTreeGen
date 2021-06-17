#ifndef TTF_H 
#define TTF_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class SymbolButton: public wxButton 
{
    wxTextCtrl *&currCtrl;
    long &lastCursorPosition;

public:
    SymbolButton(wxFrame *frame, int id, wxString specialChar, wxTextCtrl *&currCtrl, long &cp);

    void handleClick(wxCommandEvent &ce);
};


class TruthTreeFrame: public wxFrame 
{
    wxTextCtrl *argCtrl, *concCtrl;
    wxTextCtrl *currCtrl;

    SymbolButton* charBtns[9];
    long lastCursorPosition;

    wxButton generateTreeBtn;

public:
    TruthTreeFrame();

    void handleTxtCtrl(wxFocusEvent &fe);
    void handleCharBtn(wxFocusEvent &fe);

    void generateTree(wxCommandEvent &ce);
};


#endif