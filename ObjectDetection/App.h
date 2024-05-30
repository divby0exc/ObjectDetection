#pragma once
#include <wx/wx.h>

class App : public wxApp {
	wxDateTime session_exp_time;
	wxTimer sess_timer;
public:
	bool OnInit();
	void set_session_expiration_time(const wxDateTime& expirationTime);
	void on_timer(wxTimerEvent& event);
	void logout_user();
};