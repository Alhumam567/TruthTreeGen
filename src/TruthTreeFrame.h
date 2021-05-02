#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifndef TTF_H 
#define TTF_H

class SymbolButton: public wxButton 
{
    wxTextCtrl *&currCtrl;
    long &cursorPosition;
    
public:
    SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt, wxTextCtrl *&currCtrl, long &cp);

    void handleClick(wxCommandEvent &ce);
};

class TruthTreeFrame: public wxFrame 
{
    wxTextCtrl *argCtrl, *concCtrl;
    wxTextCtrl *currCtrl;
    SymbolButton* charBtns[9];
    long cursorPosition;

public:
    TruthTreeFrame();

    wxTextCtrl *getCurrCtrl();
    void setCurrCtrl(wxTextCtrl *txt);
    void updateTxtCtrlCursorPosition();

    boolean isTextCtrl(wxObject *obj);
    boolean isSpecialCharBtn(wxObject *obj);
};

#endif