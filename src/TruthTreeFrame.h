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

#endif