#pragma once
#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <wx/aui/auibook.h>
#include <fstream>
#include <wx/filepicker.h>

class MainView : public wxFrame {
public:
	MainView(const wxString& title);

};

class ImagePanel : public wxPanel {
public:
    ImagePanel(wxWindow* parent);
    void start_detection(wxCommandEvent& evt);
    void save_filename(wxFileDirPickerEvent& evt);
};

class WebcamPanel : public wxPanel {
public:
    WebcamPanel(wxWindow* parent);
    void start_detection(wxCommandEvent& evt);
};
