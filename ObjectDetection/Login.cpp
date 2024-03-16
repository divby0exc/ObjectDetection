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

Login::Login(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200)) {
	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* usernameSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* usernameLabel = new wxStaticText(panel, wxID_ANY, "Username:");
	wxTextCtrl* username = new wxTextCtrl(panel, wxID_ANY, "");
	usernameSizer->Add(usernameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	usernameSizer->Add(username, 1, wxEXPAND | wxALL, 5);
	vSizer->Add(usernameSizer, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* passwordLabel = new wxStaticText(panel, wxID_ANY, "Password:");
	wxTextCtrl* password = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	passwordSizer->Add(passwordLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	passwordSizer->Add(password, 1, wxEXPAND | wxALL, 5);
	vSizer->Add(passwordSizer, 0, wxEXPAND | wxALL, 5);

	wxButton* loginBtn = new wxButton(panel, wxID_ANY, "Login");
	vSizer->Add(loginBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	wxButton* registerBtn = new wxButton(panel, wxID_ANY, "Register");
	vSizer->Add(registerBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	panel->SetSizer(vSizer);

	Centre();
}

void Login::set_username(wxCommandEvent& evt)
{
	username= evt.GetString();
	evt.Skip();
}

void Login::set_pwd(wxCommandEvent& evt)
{
	password = evt.GetString();
	evt.Skip();
}

std::string Login::get_username()
{
	return username;
}

std::string Login::get_pwd()
{
	return password;
}

void Login::show_shit(wxCommandEvent& evt) {
	Poco::URI uri("http://127.0.0.1:5000/login");
	try {
		Poco::JSON::Object::Ptr user = new Poco::JSON::Object;
		user->set("username", get_username());
		user->set("password", get_pwd());

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

		wxMessageBox(json_str, "Response: ", wxOK | wxICON_INFORMATION);
	}
	catch (Poco::Exception& ex) {
		wxMessageBox("Poco Exception: " + ex.displayText());
	}


}
