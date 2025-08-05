#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <wx/wx.h>

#include <memory>
#include <functional>
#include <string>

class MainWindow : public wxFrame
{
public:
	MainWindow();
	~MainWindow() override = default;

	static MainWindow& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	MainWindow(MainWindow&&) = delete;
	MainWindow& operator=(MainWindow&&) = delete;

	void UpdateDisplay(const std::string& value);
	void SetStatusMessage(const std::string& message);
	
private:

	static std::unique_ptr<MainWindow> instance_;

	wxTextCtrl* display_;
	wxPanel* mainPanel_;
	wxStaticText* statusLabel_;

	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnThemeToggle(wxCommandEvent& event);
	void OnFullScreen(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);

	void CreateMenuBar();
	void CreateUI();
	void SetupLayout();
	void SetupEventHandlers();
	void ApplyModernStyle();
	void SetDarkTheme(bool dark = true);

	bool isDarkTheme_;
	bool isFullscreen_;

	enum IDs
	{
		ID_ABOUT = wxID_ABOUT,
		ID_EXIT = wxID_EXIT,
		ID_THEME_TOGGLE = 2000,
		ID_FULLSCREEN = 2001
	};

};

#endif //MAIN_WINDOW_H