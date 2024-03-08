#include <wx/wx.h>
#include "MainView.h"
// OpenFileDialog for choosing picture to process

// NotificationMessage when a person enters wrong username or pwd

// MessageDialog to present wrong login info and try again or register button after 3 tries?

// GenericProgressDialog to show while detecting objects in the image

// Others > FrameIcon for a custom icon

MainView::MainView(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	wxImage img(wxT(), wxBITMAP_TYPE_ANY);
	if (img.IsOk()) {
		wxBitmap bitmap(img);

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