#include <wx/wx.h>
#include "MainView.h"
#include <iostream>
#include "ImageClass.h"

ImageClass model;
static std::string filename;

// Others > FrameIcon for a custom icon

MainView::MainView(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    ImagePanel* imagePanel = new ImagePanel(notebook);
    notebook->AddPage(imagePanel, "Image", true);

    WebcamPanel* webcamPanel = new WebcamPanel(notebook);
    notebook->AddPage(webcamPanel, "Webcam", false);

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
    //model.set_image(filename);
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

void AdminPanel::change_password(wxCommandEvent& evt)
{
}

void AdminPanel::delete_user(wxCommandEvent& evt)
{
}

std::vector<User> AdminPanel::get_users()
{
    return std::vector<User>();
}
