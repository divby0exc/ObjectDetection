#include <wx/wx.h>
#include "MainView.h"
#include <iostream>
#include "ImageClass.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>

ImageClass model;
static std::string filename;

// Others > FrameIcon for a custom icon

MainView::MainView(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

	ImagePanel* image_panel = new ImagePanel(notebook);
	notebook->AddPage(image_panel, "Image", true);

	/*WebcamPanel* webcam_panel = new WebcamPanel(notebook);
	notebook->AddPage(webcam_panel, "Webcam", false);*/

	UserPanel* user_panel = new UserPanel(notebook);
	notebook->AddPage(user_panel, "Profile", false);

}

ImagePanel::ImagePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxButton* detect_button = new wxButton(this, wxID_ANY, "Start Object Detection");
	sizer->Add(detect_button, 0, wxALL | wxALIGN_CENTER, 5);
	detect_button->Bind(wxEVT_BUTTON, &ImagePanel::start_detection, this);

	//wxButton* selectImageButton = new wxButton(this, wxID_ANY, "Select Image");
	wxStaticText* select_image_text = new wxStaticText(this, wxID_ANY, "Select image");
	wxFilePickerCtrl* file_picker = new wxFilePickerCtrl(this, wxID_ANY);
	file_picker->Bind(wxEVT_FILEPICKER_CHANGED, &ImagePanel::save_filename, this);

	sizer->Add(select_image_text, 0, wxALL | wxALIGN_CENTER, 5);
	sizer->Add(file_picker, 0, wxALL | wxALIGN_CENTER, 5);

	SetSizerAndFit(sizer);


}

void ImagePanel::start_detection(wxCommandEvent& evt)
{
	if (model.get_image().empty()) {
		wxMessageBox("Please choose an image first", "Warning: ", wxOK | wxICON_INFORMATION);
		return;
	}
	std::vector<Mat> detections;
	detections = model.pre_process(model.get_image(), model.get_model());

	Mat img = model.post_process(model.get_image().clone(), detections, model.get_classes());
	std::vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = model.get_model().getPerfProfile(layersTimes) / freq;
	std::string label = format("Inference time : %.2f ms", t);
	putText(img, label, Point(20, 40), model.get_font_face(), model.get_font_scale(), model.get_red());
	imshow("Output", img);
	waitKey(0);
}

void ImagePanel::save_filename(wxFileDirPickerEvent& evt)
{
	model.set_image((std::string)evt.GetPath());
}

WebcamPanel::WebcamPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxButton* start_button = new wxButton(this, wxID_ANY, "Start Webcam Broadcast");
	start_button->Bind(wxEVT_BUTTON, &WebcamPanel::start_detection, this);
	sizer->Add(start_button, 0, wxALL | wxALIGN_CENTER, 5);

	SetSizerAndFit(sizer);
}

void WebcamPanel::start_detection(wxCommandEvent& evt)
{
	try
	{
		VideoCapture(0);
	}
	catch (Exception e) {
		std::cout << e.what() << std::endl;
	}
}

AdminPanel::AdminPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);


}

UserPanel::UserPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* username_txt = new wxStaticText(this, wxID_ANY, "Username");
	sizer->Add(username_txt, 0, wxALL | wxALIGN_CENTER, 5);

	wxTextCtrl* username = new wxTextCtrl(this, wxID_ANY, "");
	username->Bind(wxEVT_TEXT, &UserPanel::set_username, this);
	sizer->Add(username, 0, wxALL | wxALIGN_CENTER, 5);
	
	wxStaticText* old_pwd_txt = new wxStaticText(this, wxID_ANY, "Old password");
	sizer->Add(old_pwd_txt, 0, wxALL | wxALIGN_CENTER, 5);

	wxTextCtrl* old_pwd = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	old_pwd->Bind(wxEVT_TEXT, &UserPanel::set_old_pwd, this);
	sizer->Add(old_pwd, 0, wxALL | wxALIGN_CENTER, 5);
	
	wxStaticText* new_pwd_txt = new wxStaticText(this, wxID_ANY, "New password");
	sizer->Add(new_pwd_txt, 0, wxALL | wxALIGN_CENTER, 5);

	wxTextCtrl* new_pwd = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	new_pwd->Bind(wxEVT_TEXT, &UserPanel::set_new_pwd, this);
	sizer->Add(new_pwd, 0, wxALL | wxALIGN_CENTER, 5);

	wxStaticText* re_type_pwd_txt = new wxStaticText(this, wxID_ANY, "Re-type password");
	sizer->Add(re_type_pwd_txt, 0, wxALL | wxALIGN_CENTER, 5);

	wxTextCtrl* re_pwd = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	re_pwd->Bind(wxEVT_TEXT, &UserPanel::set_re_pwd, this);
	sizer->Add(re_pwd, 0, wxALL | wxALIGN_CENTER, 5);

	wxButton* change_pwd = new wxButton(this, wxID_ANY, "Change password");
	change_pwd->Bind(wxEVT_BUTTON, &UserPanel::change_pwd, this);
	sizer->Add(change_pwd, 0, wxALL | wxALIGN_CENTER, 5);

	SetSizerAndFit(sizer);
}

void UserPanel::change_pwd(wxCommandEvent& evt)
{
	if (!is_user()) { wxMessageBox("Username or Password is incorrect", "Error: ", wxOK | wxICON_INFORMATION); return; };
	
	if(is_user() && is_pwd_same())
	{
		Poco::URI uri("http://127.0.0.1:5000/change_pwd");
		try {
			Poco::JSON::Object::Ptr user = new Poco::JSON::Object;
			user->set("username", username);
			user->set("password", new_pwd);

			std::ostringstream data;
			Poco::JSON::Stringifier::stringify(user, data);
			std::string json_data = data.str();

			Poco::Net::HTTPClientSession sess(uri.getHost(), uri.getPort());
			Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_PUT, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
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
			wxString response_req = obj->getValue<std::string>("Msg");
			wxMessageBox(response_req, "Response: ", wxOK | wxICON_INFORMATION);
		}
		catch (Poco::Exception& ex) {
			wxMessageBox("Poco Exception: " + ex.displayText());
		}
	}
}

void UserPanel::set_old_pwd(wxCommandEvent& evt)
{
	old_pwd = evt.GetString();
	evt.Skip();
}

void UserPanel::set_new_pwd(wxCommandEvent& evt)
{
	new_pwd = evt.GetString();
	evt.Skip();
}

void UserPanel::set_re_pwd(wxCommandEvent& evt)
{
	re_pwd = evt.GetString();
	evt.Skip();
}

void UserPanel::set_username(wxCommandEvent& evt)
{
	username = evt.GetString();
}

void UserPanel::set_session_time(int time)
{
	session_time = time;
}

void UserPanel::delete_my_acc(wxCommandEvent& evt)
{

}

bool UserPanel::is_pwd_same()
{
	return (new_pwd == re_pwd);
}

bool UserPanel::is_user()
{
	Poco::URI uri("http://127.0.0.1:5000/login");
	try {
		Poco::JSON::Object::Ptr user = new Poco::JSON::Object;
		user->set("username", username);
		user->set("password", old_pwd);

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
		if (obj->has("time")) {
			return true;
		}
		else return false;
	}
	catch (Poco::Exception& ex) {
		wxMessageBox("Poco Exception: " + ex.displayText());
	}
}
