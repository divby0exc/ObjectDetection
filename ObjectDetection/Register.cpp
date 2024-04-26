#include <wx/wx.h>
#include "Register.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>
#include "User.h"

static User user_obj;

Register::Register(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxPanel* panel = new wxPanel(this);
	wxButton* register_user = new wxButton(panel, wxID_ANY, "Register", wxPoint(10, 220), wxSize(100, 50));
	register_user->Bind(wxEVT_BUTTON, &Register::register_user, this);

	wxButton* abort_btn = new wxButton(panel, wxID_ANY, "Abort", wxPoint(190, 220), wxSize(100, 50));
	abort_btn->Bind(wxEVT_BUTTON, &Register::clear_fields, this);

	wxStaticText* username_txt = new wxStaticText(panel, wxID_ANY, "Username", wxPoint(130, 10));
	username_in = new wxTextCtrl(panel, 1, "", wxPoint(108, 30));
	username_in->Bind(wxEVT_TEXT, &Register::set_username, this);

	wxStaticText* password_txt = new wxStaticText(panel, wxID_ANY, "Password", wxPoint(130, 100));
	password_in = new wxTextCtrl(panel, 2, "", wxPoint(108, 120), wxDefaultSize, wxTE_PASSWORD);
	password_in->Bind(wxEVT_TEXT, &Register::set_password, this);
}	

void Register::clear_fields(wxCommandEvent& evt)
{
	username_in->Clear();
	password_in->Clear();
}

void Register::set_username(wxCommandEvent& evt) {
	user_obj.set_username((std::string)evt.GetString());
	evt.Skip();
}

void Register::set_password(wxCommandEvent& evt) {
	user_obj.set_password((std::string)evt.GetString());
	evt.Skip();
}

void Register::register_user(wxCommandEvent& evt)
{
	Poco::URI uri("http://127.0.0.1:5000/register");
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

		wxMessageBox(json_str, "Response: ", wxOK | wxICON_INFORMATION);
	}
	catch (Poco::Exception& ex) {
		wxMessageBox("Poco Exception: " + ex.displayText());
	}
}
