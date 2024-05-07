#pragma once
#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <wx/aui/auibook.h>

class MainView : public wxFrame {
public:
	MainView(const wxString& title);

};

class ImagePanel : public wxPanel {
public:
    ImagePanel(wxWindow* parent);
};

class WebcamPanel : public wxPanel {
public:
    WebcamPanel(wxWindow* parent);
};
