#include "core/app.h"
#include "ui/main_window.h"

bool CalculatorApp::OnInit()
{
	MainWindow::CreateInstance();
	MainWindow& mainWindow = MainWindow::GetInstance();

	mainWindow.Show(true);

	SetTopWindow(&mainWindow);
	
	return true;
}

int CalculatorApp::OnExit()
{
	MainWindow::DestroyInstance();
	return wxApp::OnExit();
}
