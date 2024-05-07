#include <wx/wx.h>
#include "MainView.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>

using namespace cv;
using namespace dnn;
// OpenFileDialog for choosing picture to process

// NotificationMessage when a person enters wrong username or pwd

// MessageDialog to present wrong login info and try again or register button after 3 tries?

// GenericProgressDialog to show while detecting objects in the image

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

    wxButton* detectButton = new wxButton(this, wxID_ANY, "Start Object Detection");
    sizer->Add(detectButton, 0, wxALL | wxALIGN_CENTER, 5);

    wxButton* selectImageButton = new wxButton(this, wxID_ANY, "Select Image");
    sizer->Add(selectImageButton, 0, wxALL | wxALIGN_CENTER, 5);

    SetSizerAndFit(sizer);
}

WebcamPanel::WebcamPanel(wxWindow * parent) : wxPanel(parent, wxID_ANY) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxButton* startButton = new wxButton(this, wxID_ANY, "Start Webcam Broadcast");
    sizer->Add(startButton, 0, wxALL | wxALIGN_CENTER, 5);

    wxButton* stopButton = new wxButton(this, wxID_ANY, "Stop Webcam Broadcast");
    sizer->Add(stopButton, 0, wxALL | wxALIGN_CENTER, 5);

    SetSizerAndFit(sizer);
}