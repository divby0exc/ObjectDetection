#pragma once
#include <wx/wx.h>

class Register : public wxFrame {
public:
	Register(const wxString& title);
private:
	void clear_fields(wxCommandEvent& evt);
	void set_username(wxCommandEvent& evt);
	void set_password(wxCommandEvent& evt);
	void register_user(wxCommandEvent& evt);
	void goto_login(wxCommandEvent& evt);
	wxTextCtrl* username_in;
	wxTextCtrl* password_in;
};