#pragma once
#include <wx/wx.h>

class Login : public wxFrame {
public:
	Login(const wxString& title);
private:
	void set_username(wxCommandEvent& evt);
	void set_pwd(wxCommandEvent& evt);
	void show_shit(wxCommandEvent& evt);
	void register_view(wxCommandEvent& evt);
};