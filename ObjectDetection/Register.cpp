#include <wx/wx.h>
#include "Register.h"

Register::Register(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	wxButton* register_user = new wxButton(panel, wxID_ANY, "Register", wxPoint(10, 220), wxSize(100, 50));

	wxButton* abort_btn = new wxButton(panel, wxID_ANY, "Abort", wxPoint(190, 220), wxSize(100, 50));
	abort_btn->Bind(wxEVT_BUTTON, &Register::clear_fields, this);

	wxStaticText* username_txt = new wxStaticText(panel, wxID_ANY, "Username", wxPoint(130, 10));
	username_in = new wxTextCtrl(panel, 1, "", wxPoint(108, 30));

	wxStaticText* password_txt = new wxStaticText(panel, wxID_ANY, "Password", wxPoint(130, 100));
	password_in = new wxTextCtrl(panel, 2, "", wxPoint(108, 120));
}

void Register::clear_fields(wxCommandEvent& evt)
{
	username_in->Clear();
	password_in->Clear();
}
