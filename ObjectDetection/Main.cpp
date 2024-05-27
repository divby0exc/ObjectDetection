#include "Login.h"
#include "App.h"
#include "Register.h"
#include "MainView.h"
wxIMPLEMENT_APP(App);
bool App::OnInit() {
	/*Login* login = new Login("Login");
	login->SetClientSize(300, 300);
	login->Center();
	login->Show();*/
	//Register* register_frame = new Register("Register");
	//register_frame->SetClientSize(300, 300);
	//register_frame->Center();
	//register_frame->Show();
	MainView* main_view = new MainView("ObjectDetection");
	main_view->SetClientSize(300, 300);
	main_view->Center();
	main_view->Show();



	return true;


}