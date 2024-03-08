#include "Login.h"
#include "App.h"
wxIMPLEMENT_APP(App);
bool App::OnInit() {
	Login* login = new Login("Login");
	login->SetClientSize(300, 300);
	login->Center();
	login->Show();
	return true;
}