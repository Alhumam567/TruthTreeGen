#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifndef TTF_H 
#define TTF_H

class TruthTreeFrame: public wxFrame 
{
    wxTextCtrl *argCtrl, *concCtrl;
    wxTextCtrl *currCtrl;

public:
    TruthTreeFrame();

    wxTextCtrl *getCurrCtrl();
    void setCurrCtrl(wxTextCtrl *txt);

    boolean isTextCtrl(wxObject *obj);
};

class SymbolButton: public wxButton 
{
    wxTextCtrl *&currCtrl;

public:
    SymbolButton(wxFrame *frame, wxString specialChar, wxTextCtrl *txt, wxTextCtrl *&currCtrl);

    void handleClick(wxCommandEvent &ce);
};


#endif