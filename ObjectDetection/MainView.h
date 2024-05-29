#pragma once
#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <wx/aui/auibook.h>
#include <fstream>
#include <wx/filepicker.h>
#include "User.h"

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

class AdminPanel : public wxPanel {
public:
    AdminPanel(wxWindow* parent);
    void change_password(wxCommandEvent& evt);
    void delete_user(wxCommandEvent& evt);
    std::vector<User> get_users();
};

class UserPanel : public wxPanel {
    std::string username;
    std::string old_pwd;
    std::string new_pwd;
    std::string re_pwd;
    int session_time;
public:
    UserPanel(wxWindow* parent);
    void change_pwd(wxCommandEvent& evt);
    void set_old_pwd(wxCommandEvent& evt);
    void set_new_pwd(wxCommandEvent& evt);
    void set_re_pwd(wxCommandEvent& evt);
    void set_username(wxCommandEvent& evt);
    void set_session_time(int time);
    void delete_my_acc(wxCommandEvent& evt);
    bool is_pwd_same();
    bool is_user();
};
