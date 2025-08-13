#include "ui/main_window.h"
#include <wx/msgdlg.h>
#include <wx/menu.h>

MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, id, title, pos, size)
    , m_display(nullptr)
    , m_mainPanel(nullptr)
    , m_statusLabel(nullptr)
    , m_isDarkTheme(false)
    , m_isFullscreen(false)
    , m_waitingForOperand(true)
    , m_hasDecimal(false)
{
    SetMinSize(wxSize(MIN_WIDTH, MIN_HEIGHT));

    CreateMenuBar();
    CreateUI();
    SetupLayout();
    SetupEventHandlers();
    ApplyModernStyle();

    Centre();

    if (m_display) 
    {
        m_display->SetFocus();
    }
}

void MainWindow::CreateUI()
{
    m_mainPanel = new wxPanel(this, wxID_ANY);
    m_mainPanel->SetBackgroundColour(wxColour(240, 240, 240));

    m_display = new wxTextCtrl(m_mainPanel, wxID_ANY, "0",
        wxDefaultPosition, wxSize(-1, DISPLAY_HEIGHT),
        wxTE_RIGHT | wxTE_READONLY | wxBORDER_NONE);

    wxFont displayFont = m_display->GetFont();
    displayFont.SetPointSize(24);
    displayFont.SetWeight(wxFONTWEIGHT_LIGHT);
    m_display->SetFont(displayFont);
    m_display->SetBackgroundColour(wxColour(50, 50, 50));
    m_display->SetForegroundColour(wxColour(255, 255, 255));

    m_buttonPanel = std::make_unique<ButtonPanel>(m_mainPanel);

    m_statusLabel = new wxStaticText(m_mainPanel, wxID_ANY, "Ready",
        wxDefaultPosition, wxSize(-1, STATUS_HEIGHT),
        wxALIGN_LEFT);
    m_statusLabel->SetBackgroundColour(wxColour(240, 240, 240));
}

void MainWindow::SetupLayout()
{
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(m_display, 0, wxEXPAND | wxALL, 10);

    mainSizer->Add(m_buttonPanel.get(), 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    mainSizer->Add(m_statusLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    m_mainPanel->SetSizer(mainSizer);

    auto* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(m_mainPanel, 1, wxEXPAND);
    SetSizer(frameSizer);
}

void MainWindow::SetupEventHandlers()
{
    Bind(EVT_CALC_NUMBER, &MainWindow::OnNumber, this);
    Bind(EVT_CALC_OPERATOR, &MainWindow::OnOperator, this);
    Bind(EVT_CALC_EQUALS, &MainWindow::OnEquals, this);
    Bind(EVT_CALC_CLEAR, &MainWindow::OnClear, this);
    Bind(EVT_CALC_CLEAR_ENTRY, &MainWindow::OnClearEntry, this);
    Bind(EVT_CALC_DECIMAL, &MainWindow::OnDecimal, this);
    Bind(EVT_CALC_BACKSPACE, &MainWindow::OnBackspace, this);

    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
    Bind(wxEVT_KEY_DOWN, &MainWindow::OnKeyDown, this);
    Bind(wxEVT_SIZE, &MainWindow::OnSize, this);

    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, ID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, ID_EXIT);
    Bind(wxEVT_MENU, &MainWindow::OnThemeToggle, this, ID_THEME_TOGGLE);
    Bind(wxEVT_MENU, &MainWindow::OnFullScreen, this, ID_FULLSCREEN);
}

void MainWindow::OnNumber(wxCommandEvent& event)
{
    wxString number = event.GetString();

    if (m_waitingForOperand) 
    {
        m_currentNumber = number;
        m_waitingForOperand = false;
        m_hasDecimal = false;
    }
    else 
    {
        if (m_currentNumber == "0") 
        {
            m_currentNumber = number;
        }
        else 
        {
            m_currentNumber += number;
        }
    }

    UpdateDisplay(m_currentNumber);
    SetStatusMessage("Number input: " + number);
}

void MainWindow::OnOperator(wxCommandEvent& event)
{
    wxString op = event.GetString();

    if (!m_waitingForOperand) 
    {
 
        if (!m_currentOperator.IsEmpty() && !m_previousNumber.IsEmpty()) 
        {
            OnEquals(event);  
        }
        else 
        {
            m_previousNumber = m_currentNumber;
        }
    }

    m_currentOperator = op;
    m_waitingForOperand = true;

    SetStatusMessage("Operator: " + op);
}

void MainWindow::OnEquals(wxCommandEvent& event)
{
    if (m_currentOperator.IsEmpty() || m_previousNumber.IsEmpty()) {
        return;
    }

    double prev = 0.0, curr = 0.0, result = 0.0;

    if (!m_previousNumber.ToDouble(&prev) || !m_currentNumber.ToDouble(&curr)) {
        SetDisplayError("Error");
        return;
    }

    if (m_currentOperator == "+") 
    {
        result = prev + curr;
    }
    else if (m_currentOperator == "-") 
    {
        result = prev - curr;
    }
    else if (m_currentOperator == "*") 
    {
        result = prev * curr;
    }
    else if (m_currentOperator == "/") 
    {
        if (curr == 0.0) {
            SetDisplayError("Division by zero");
            return;
        }
        result = prev / curr;
    }

    m_currentNumber = wxString::Format("%.10g", result);
    UpdateDisplay(m_currentNumber);

    m_previousNumber.Clear();
    m_currentOperator.Clear();
    m_waitingForOperand = true;

    SetStatusMessage("Calculation completed");
}

void MainWindow::OnClear(wxCommandEvent& event)
{
    m_currentNumber = "0";
    m_previousNumber.Clear();
    m_currentOperator.Clear();
    m_waitingForOperand = true;
    m_hasDecimal = false;

    UpdateDisplay(m_currentNumber);
    SetStatusMessage("Cleared");
}

void MainWindow::OnClearEntry(wxCommandEvent& event)
{
    m_currentNumber = "0";
    m_hasDecimal = false;
    UpdateDisplay(m_currentNumber);
    SetStatusMessage("Entry cleared");
}

void MainWindow::OnDecimal(wxCommandEvent& event)
{
    if (m_waitingForOperand) 
    {
        m_currentNumber = "0.";
        m_waitingForOperand = false;
        m_hasDecimal = true;
    }
    else if (!m_hasDecimal) 
    {
        m_currentNumber += ".";
        m_hasDecimal = true;
    }

    UpdateDisplay(m_currentNumber);
}

void MainWindow::OnBackspace(wxCommandEvent& event)
{
    if (m_currentNumber.Length() > 1) {
        if (m_currentNumber.Last() == '.') 
        {
            m_hasDecimal = false;
        }
        m_currentNumber = m_currentNumber.Left(m_currentNumber.Length() - 1);
    }
    else 
    {
        m_currentNumber = "0";
        m_hasDecimal = false;
    }

    UpdateDisplay(m_currentNumber);
}

void MainWindow::UpdateDisplay(const wxString& value)
{
    if (m_display) 
    {
        m_display->SetValue(value);
    }
}

void MainWindow::SetStatusMessage(const wxString& message)
{
    if (m_statusLabel) 
    {
        m_statusLabel->SetLabel(message);
    }
}

void MainWindow::SetDisplayError(const wxString& errorMsg)
{
    UpdateDisplay(errorMsg);
    SetStatusMessage("Error occurred");

    m_currentNumber = "0";
    m_previousNumber.Clear();
    m_currentOperator.Clear();
    m_waitingForOperand = true;
    m_hasDecimal = false;
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Modern Calculator\nBuilt with wxWidgets and C++",
        "About Calculator", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnClose(wxCloseEvent& event)
{
    Destroy();
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    Layout();
    event.Skip();
}

void MainWindow::CreateMenuBar() 
{ /* Это ты напишешь сам  */ 

}


void MainWindow::OnThemeToggle(wxCommandEvent& event) 
{ /* и это */ 

}

void MainWindow::OnFullScreen(wxCommandEvent& event) 
{ /* и это */ 

}

void MainWindow::OnKeyDown(wxKeyEvent& event) 
{ /* и это */ 

}

void MainWindow::ApplyModernStyle() 
{ /* и это */ 

}

void MainWindow::SetDarkTheme(bool dark) 
{ /* и это тоже, тебе всё равно делать нехуй */  

}