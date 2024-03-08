#include "Login.h"
#include "App.h"
#include "MainView.h"
wxIMPLEMENT_APP(App);
bool App::OnInit() {
	/*Login* login = new Login("Login");
	login->SetClientSize(300, 300);
	login->Center();
	login->Show();*/
	MainView* main = new MainView("Main");
	main->SetClientSize(800, 600);
	main->Center();
	main->Show();
	return true;


}