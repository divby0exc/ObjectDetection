#include "Login.h"
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <iostream>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>
#include "Register.h"
#include "User.h"
#include "MainView.h"

static User user_obj;

Login::Login(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);

	wxStaticText* username_text = new wxStaticText(panel, wxID_ANY, "Username", wxPoint(50, 10));
	wxTextCtrl* username = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(50, 30));
	username->Bind(wxEVT_TEXT, &Login::set_username, this);

	wxStaticText* password_text = new wxStaticText(panel, wxID_ANY, "Password", wxPoint(50, 60));
	wxTextCtrl* password = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(50, 80), wxDefaultSize, wxTE_PASSWORD);
	password->Bind(wxEVT_TEXT, &Login::set_pwd, this);

	wxButton* login = new wxButton(panel, wxID_ANY, "Login", wxPoint(50,110));
	login->Bind(wxEVT_BUTTON, &Login::show_shit, this);

	wxButton* register_btn = new wxButton(panel, wxID_ANY, "Register", wxPoint(175, 110));
	register_btn->Bind(wxEVT_BUTTON, &Login::register_view, this);

	

}

void Login::set_username(wxCommandEvent& evt)
{
	if (evt.GetString().length()) {
		if (!evt.GetString().length() < 6) {
			user_obj.set_username((std::string)evt.GetString());
			evt.Skip();
			return;
		}
	}
	wxMessageBox("Username too short", "Response: ", wxOK | wxICON_INFORMATION);
}

void Login::set_pwd(wxCommandEvent& evt)
{
	if(evt.GetString().length()){
		if (!evt.GetString().Length() < 10)
			user_obj.set_password((std::string)evt.GetString());
			evt.Skip();
			return;
	}
	wxMessageBox("Password too short", "Response: ", wxOK | wxICON_INFORMATION);
}


void Login::show_shit(wxCommandEvent& evt) {
	Poco::URI uri("http://127.0.0.1:5000/login");
	try {
		Poco::JSON::Object::Ptr user = new Poco::JSON::Object;
		user->set("username", user_obj.get_username());
		user->set("password", user_obj.get_password());

		std::ostringstream data;
		Poco::JSON::Stringifier::stringify(user, data);
		std::string json_data = data.str();

		Poco::Net::HTTPClientSession sess(uri.getHost(), uri.getPort());
		Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
		req.setContentType("application/json");
		req.setContentLength(json_data.length());

		std::ostream& req_stream = sess.sendRequest(req);
		req_stream << json_data;

		Poco::Net::HTTPResponse res;
		std::istream& res_stream = sess.receiveResponse(res);

		std::stringstream ss;
		Poco::StreamCopier::copyStream(res_stream, ss);
		std::string res_data = ss.str();

		Poco::JSON::Parser parser;
		Poco::Dynamic::Var json_res = parser.parse(res_data);
		Poco::JSON::Object::Ptr obj = json_res.extract<Poco::JSON::Object::Ptr>();

		std::ostringstream oss;
		obj->stringify(oss);


		wxString json_str(oss.str());

		// need to find out how to extract the keys in best way
		if (json_str.find("true")) {
			// Save the seconds in a thread or something to keep track to logout
			MainView* main_view = new MainView("Object Detection");
			main_view->SetClientSize(1200, 800);
			main_view->Center();
			main_view->Show();
			this->Close();
			// Awful solution but works because of the demand of sending/receiving json str
		} else wxMessageBox(json_str.substr(json_str.find("Username"), 33), "Response: ", wxOK | wxICON_INFORMATION);
	}
	catch (Poco::Exception& ex) {
		wxMessageBox("Poco Exception: " + ex.displayText());
	}


}

void Login::register_view(wxCommandEvent& evt) {
	Register* register_frame = new Register("Register");
	register_frame->SetClientSize(300, 300);
	register_frame->Center();
	register_frame->Show();
	this->Close();
}
