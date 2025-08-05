#include "ui/main_window.h"
#include "wx/artprov.h"
#include "wx/display.h"

std::unique_ptr<MainWindow> MainWindow::instance_ = nullptr;

MainWindow& MainWindow::GetInstance()
{
	if (!instance_)
	{
		CreateInstance();
	}

	return *instance_;
}

void MainWindow::CreateInstance()
{
	if (!instance_)
	{
		instance_ = std::unique_ptr<MainWindow>(new MainWindow());
	}
}

void MainWindow::DestroyInstance()
{
	instance_.reset();
}

MainWindow::MainWindow()
	:wxFrame(nullptr, wxID_ANY, "Modern Calculator",
		wxDefaultPosition, wxSize(450, 600),
		wxDEFAULT_FRAME_STYLE)
	, display_(nullptr)
	, mainPanel_(nullptr)
	, statusLabel_(nullptr)
	, isDarkTheme_(true)
	, isFullscreen_(false)
{
	CreateMenuBar();
	CreateUI();
	SetupLayout();
	SetupEventHandlers();
	ApplyModernStyle();

	CenterOnScreen();

	SetMinSize(wxSize(350, 500));

	SetIcon(wxArtProvider::GetIcon(wxART_EXECUTABLE_FILE, wxART_FRAME_ICON));
}

void MainWindow::CreateMenuBar()
{
	auto menuBar = std::make_unique<wxMenuBar>();

	auto fileMenu = std::make_unique<wxMenu>();
	fileMenu->Append(ID_FULLSCREEN, "&Fullscreen\tF11", "Toggle fullscreen mode");
	fileMenu->AppendSeparator();
	fileMenu->Append(ID_EXIT, "E&xit\tCtrl+Q", "Exit the application");

	auto viewMenu = std::make_unique<wxMenu>();
	viewMenu->AppendCheckItem(ID_THEME_TOGGLE, "$Dark Theme\tCtrl+T", "Toggle dark/light theme");
	viewMenu->Check(ID_THEME_TOGGLE, isDarkTheme_);

	auto helpMenu = std::make_unique<wxMenu>();
	helpMenu->Append(ID_ABOUT, "&About\tF1", "Show about dialog");

	menuBar->Append(fileMenu.release(), "&File");
	menuBar->Append(viewMenu.release(), "&View");
	menuBar->Append(helpMenu.release(), "&Help");

}

void MainWindow::CreateUI()
{
	mainPanel_ = new wxPanel(this, wxID_ANY);

	display_ = new wxTextCtrl(mainPanel_, wxID_ANY, "0",
		wxDefaultPosition, wxDefaultSize,
		wxTE_RIGHT | wxTE_READONLY | wxBORDER_SIMPLE);

	statusLabel_ = new wxStaticText(mainPanel_, wxID_ANY, "Ready",
		wxDefaultPosition, wxDefaultSize,
		wxALIGN_CENTER_HORIZONTAL);
}

void MainWindow::SetupLayout()
{
	auto mainSizer = std::make_unique<wxBoxSizer>(wxVERTICAL);

	mainSizer->Add(display_, 0, wxEXPAND | wxLEFT | wxBOTTOM, 20);

	//mainSizer->Add(buttonPanel_, 1, wxExpand | wxLeft | wxRight, 20);

	mainSizer->AddStretchSpacer(1);

	mainSizer->Add(statusLabel_, 0, wxEXPAND | wxALL, 10);

	mainPanel_->SetSizer(mainSizer.release());
}

void MainWindow::SetupEventHandlers()
{
	Bind(wxEVT_MENU, &MainWindow::OnExit, this, ID_EXIT);
	Bind(wxEVT_MENU, &MainWindow::OnAbout, this, ID_ABOUT);
	Bind(wxEVT_MENU, &MainWindow::OnThemeToggle, this, ID_THEME_TOGGLE);
	Bind(wxEVT_MENU, &MainWindow::OnFullScreen, this, ID_FULLSCREEN);

	Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);

	Bind(wxEVT_KEY_DOWN, &MainWindow::OnKeyDown, this);

	Bind(wxEVT_SIZE, [this](wxSizeEvent& event)
		{
			SetStatusMessage("Window resized to " +
				std::to_string(event.GetSize().GetWidth()) + "x" +
				std::to_string(event.GetSize().GetHeight()));

			event.Skip();
		});

	Bind(wxEVT_ACTIVATE, [this](wxActivateEvent& event)
		{
			if (event.GetActive())
			{
				SetStatusMessage("Calculator activated");
			}
			else
			{
				SetStatusMessage("Calculator deactivated");
			}

			event.Skip();
		});

	if (display_)
	{
		display_->Bind(wxEVT_SET_FOCUS, [this](wxFocusEvent& event)
			{
				SetStatusMessage("Display focused");
				event.Skip();
			});

		display_->Bind(wxEVT_KILL_FOCUS, [this](wxFocusEvent& event)
			{
				SetStatusMessage("Display unfocused");
				event.Skip();
			});
	}
	
}

void MainWindow::ApplyModernStyle()
{
	SetDarkTheme(isDarkTheme_);

	wxFont displayFont(20, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	display_->SetFont(displayFont);
	
	wxFont statusFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	statusLabel_->SetFont(statusFont);
}

void MainWindow::SetDarkTheme(bool dark)
{
	isDarkTheme_ = dark;

	if (dark)
	{
		wxColor bgColor(45, 45, 48);
		wxColor fgColor(255, 255, 255);
		wxColor displayBg(30, 30, 30);

		SetBackgroundColour(bgColor);
		mainPanel_->SetBackgroundColour(bgColor);
		mainPanel_->SetForegroundColour(fgColor);

		display_->SetBackgroundColour(displayBg);
		display_->SetForegroundColour(fgColor);

		statusLabel_->SetForegroundColour(wxColor(170, 170, 170));
	}
	else
	{
		wxColour bgColor(248, 248, 248);
		wxColour fgColor(0, 0, 0);
		wxColour displayBg(255, 255, 255);

		SetBackgroundColour(bgColor);
		mainPanel_->SetBackgroundColour(bgColor);
		mainPanel_->SetForegroundColour(fgColor);

		display_->SetBackgroundColour(displayBg);
		display_->SetForegroundColour(fgColor);

		statusLabel_->SetForegroundColour(wxColour(100, 100, 100));
	}

	Refresh();
}

void MainWindow::UpdateDisplay(const std::string& value)
{
	if (display_)
	{
		display_->SetValue(wxString(value));
	}
}

void MainWindow::SetStatusMessage(const std::string& message)
{
	if (statusLabel_) 
	{
		statusLabel_->SetLabel(wxString(message));

		
		wxTimer* timer = new wxTimer();
		timer->Bind(wxEVT_TIMER, [this, timer](wxTimerEvent&) 
			{
			SetStatusMessage("Ready");
			delete timer;
			});

		timer->StartOnce(3000);
	}
}

void MainWindow::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
	wxString aboutText = wxString::Format(
		"Modern Calculator v1.0\n\n"
		"Built with wxWidgets %d.%d.%d\n"
		"Architecture: %s\n"
		"Compiled: %s %s\n\n"
		"Features:\n"
		"• Singleton pattern architecture\n"
		"• Modern event handling with Bind()\n"
		"• Dark/Light theme support\n"
		"• Smart pointer usage\n"
		"• Lambda functions for simple events\n"
		"• Responsive design",
		wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER,
#ifdef __x86_64__
		"x64"
#else
		"x86"
#endif
		, __DATE__, __TIME__
	);

	wxMessageBox(aboutText, "About Modern Calculator",
		wxOK | wxICON_INFORMATION | wxCENTRE);
}

void MainWindow::OnThemeToggle(wxCommandEvent& event)
{
	SetDarkTheme(!isDarkTheme_);
	SetStatusMessage(isDarkTheme_ ? "Switched to dark theme" : "Switched to light theme");
}

void MainWindow::OnFullScreen(wxCommandEvent& event)
{
	isFullscreen_ = !isFullscreen_;
	ShowFullScreen(isFullscreen_);
	SetStatusMessage(isFullscreen_ ? "Entered fullscreen mode" : "Exited fullscreen mode");
}

void MainWindow::OnKeyDown(wxKeyEvent& event)
{
	int keyCode = event.GetKeyCode();

	if (event.ControlDown()) 
	{
		switch (keyCode) {
		case 'Q':
			Close(true);
			return;
		case 'T':
			SetDarkTheme(!isDarkTheme_);
			return;
		}
	}

	if (keyCode == WXK_F11) 
	{
		OnFullScreen(wxCommandEvent{});
		return;
	}

	if (keyCode == WXK_F1) 
	{
		OnAbout(wxCommandEvent{});
		return;
	}

	event.Skip();
}

void MainWindow::OnClose(wxCloseEvent& event)
{
	SetStatusMessage("Closing application...");

	if (event.CanVeto())
	{
		Hide();
	}
	else
	{
		DestroyInstance();
		Destroy();
	}
}