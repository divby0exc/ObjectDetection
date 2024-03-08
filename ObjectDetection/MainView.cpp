#include <wx/wx.h>
#include "MainView.h"
#include <iostream>
// OpenFileDialog for choosing picture to process

// NotificationMessage when a person enters wrong username or pwd

// MessageDialog to present wrong login info and try again or register button after 3 tries?

// GenericProgressDialog to show while detecting objects in the image

// Others > FrameIcon for a custom icon

MainView::MainView(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	std::cout << "Starting frame" << std::endl;
	wxImage img(wxT("IMG-20240211-WA0024.jpg"));
	std::cout << "Managed to insert pic to wxImage" << std::endl;
	if (img.IsOk()) {
		std::cout << "Image seems ok" << std::endl;
		wxBitmap bitmap(img);
		std::cout << "Converted imaged to a wxBitmap" << std::endl;

		wxStaticBitmap* static_bit = new wxStaticBitmap(this, wxID_ANY, bitmap);


		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(static_bit, 1, wxEXPAND | wxALL, 10);
		SetSizer(sizer);
	}
	else
	{
		wxMessageBox(wxT("Failed to load image."), wxT("Error"), wxOK | wxICON_ERROR);
	}
}