#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <wx/wx.h>
#include <memory>
#include <string>
#include "ui/button_panel.h"

class MainWindow : public wxFrame
{
public:
    explicit MainWindow(wxWindow* parent = nullptr,
        wxWindowID id = wxID_ANY,
        const wxString& title = "Calculator",
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(350, 500));

    virtual ~MainWindow() = default;

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    void UpdateDisplay(const wxString& value);
    void SetStatusMessage(const wxString& message);
    void ClearDisplay();
    void SetDisplayError(const wxString& errorMsg);

private:
    void OnNumber(wxCommandEvent& event);
    void OnOperator(wxCommandEvent& event);
    void OnEquals(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnClearEntry(wxCommandEvent& event);
    void OnDecimal(wxCommandEvent& event);
    void OnBackspace(wxCommandEvent& event);

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnThemeToggle(wxCommandEvent& event);
    void OnFullScreen(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnSize(wxSizeEvent& event);


    void CreateMenuBar();
    void CreateUI();
    void SetupLayout();
    void SetupEventHandlers();
    void ApplyModernStyle();
    void SetDarkTheme(bool dark = true);

    std::unique_ptr<ButtonPanel> m_buttonPanel; 
    wxTextCtrl* m_display;             
    wxPanel* m_mainPanel;              
    wxStaticText* m_statusLabel;       

    bool m_isDarkTheme;
    bool m_isFullscreen;

    wxString m_currentNumber;
    wxString m_previousNumber;
    wxString m_currentOperator;
    bool m_waitingForOperand;
    bool m_hasDecimal;

    enum IDs
    {
        ID_ABOUT = wxID_ABOUT,
        ID_EXIT = wxID_EXIT,
        ID_THEME_TOGGLE = 2000,
        ID_FULLSCREEN = 2001
    };

    static constexpr int MIN_WIDTH = 320;
    static constexpr int MIN_HEIGHT = 480;
    static constexpr int DISPLAY_HEIGHT = 60;
    static constexpr int STATUS_HEIGHT = 25;
};

#endif // MAIN_WINDOW_H