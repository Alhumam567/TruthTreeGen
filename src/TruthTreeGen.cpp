#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TruthTreeFrame.h"
#include "TruthTreePane.h"
#include "TruthTreeModel.h"

class TruthTreeGen: public wxApp
{
    TruthTreeFrame *frame;
    bool textCtrlIsChanged;

public:
    bool OnInit() {
        this->frame = new TruthTreeFrame();
        this->frame->Show(true); 
        this->textCtrlIsChanged = true;

        return true;
    }
};

IMPLEMENT_APP(TruthTreeGen);
