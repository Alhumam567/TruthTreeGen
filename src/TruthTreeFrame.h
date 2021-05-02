#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifndef TTF_H 
#define TTF_H

class SymbolButton: public wxButton 
{
    wxTextCtrl *&currCtrl;
    long &lastCursorPosition;

public:
    SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt, wxTextCtrl *&currCtrl, long &cp);

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

    wxTextCtrl *getCurrCtrl();
    void setCurrCtrl(wxTextCtrl *txt);
    void updateTxtCtrlCursorPosition();

    bool isTextCtrl(wxObject *obj);
    bool isSpecialCharBtn(wxObject *obj);

    void generateTree(wxCommandEvent &ce);
};

#endif