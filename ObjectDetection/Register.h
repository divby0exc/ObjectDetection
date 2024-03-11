#pragma once
#include <wx/wx.h>

class Register : public wxFrame {
public:
	Register(const wxString& title);
private:
	void clear_fields(wxCommandEvent& evt);
	void register_user(wxCommandEvent& evt);
	wxTextCtrl* username_in;
	wxTextCtrl* password_in;
};