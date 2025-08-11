#include "core/app.h"
#include "ui/main_window.h"

bool CalculatorApp::OnInit()
{
	MainWindow* mainWindow = new MainWindow(nullptr, wxID_ANY, "Modern Calculator",
		wxDefaultPosition, wxSize(400, 600));

	mainWindow->Show(true);

	SetTopWindow(mainWindow);
	
	return true;
}

int CalculatorApp::OnExit()
{
	return wxApp::OnExit();
}
