#include <wx/wx.h>
#include "MainView.h"
#include <iostream>
#include "ImageClass.h"

ImageClass model;
static std::string filename;

// Others > FrameIcon for a custom icon

MainView::MainView(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    ImagePanel* image_panel = new ImagePanel(notebook);
    notebook->AddPage(image_panel, "Image", true);

    WebcamPanel* webcam_panel = new WebcamPanel(notebook);
    notebook->AddPage(webcam_panel, "Webcam", false);

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
    model.set_image((std::string) evt.GetPath());
}

WebcamPanel::WebcamPanel(wxWindow * parent) : wxPanel(parent, wxID_ANY) 
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

    wxButton* change_pwd = new wxButton(this, wxID_ANY, "Change password");
    change_pwd->Bind(wxEVT_BUTTON, &UserPanel::set_pwd_flag, this);
    sizer->Add(change_pwd, 0, wxALL | wxALIGN_CENTER, 5);

    if (get_pwd_flag())
    {
        wxTextCtrl* username = new wxTextCtrl(this, wxID_ANY, "Username");
        wxTextCtrl* old_pwd = new wxTextCtrl(this, wxID_ANY, "Old password");
        wxTextCtrl* new_pwd = new wxTextCtrl(this, wxID_ANY, "New password");
        wxTextCtrl* re_pwd = new wxTextCtrl(this, wxID_ANY, "Re-type password");
        sizer->Add(username, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(old_pwd, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(new_pwd, 0, wxALL | wxALIGN_CENTER, 5);
        sizer->Add(re_pwd, 0, wxALL | wxALIGN_CENTER, 5);
    }


}

void UserPanel::set_pwd_flag(wxCommandEvent& evt)
{
    is_change_pwd = true;
}

bool UserPanel::get_pwd_flag()
{
    return is_change_pwd;
}
