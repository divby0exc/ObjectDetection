#include <wx/wx.h>

class HelloWorldFrame : public wxFrame
{
public:
    HelloWorldFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
    {
        wxPanel* panel = new wxPanel(this, wxID_ANY);
        wxStaticText* message = new wxStaticText(panel, wxID_ANY, wxT("Hello, World!"),
            wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    }
};

class HelloWorldApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        HelloWorldFrame* frame = new HelloWorldFrame(wxT("Hello, World!"));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(HelloWorldApp);
