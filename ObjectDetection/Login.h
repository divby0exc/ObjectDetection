#pragma once
#include <wx/wx.h>

class Login : public wxFrame {
public:
	Login(const wxString& title);
private:
	void set_username(wxCommandEvent& evt);
	void set_pwd(wxCommandEvent& evt);
	std::string get_username();
	std::string get_pwd();
	void show_shit(wxCommandEvent& evt);
	std::string username;
	std::string password;
};