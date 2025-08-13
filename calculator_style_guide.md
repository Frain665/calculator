# 📋 Calculator Project Code Style Guide

## 🎯 General Principles

This guide describes the coding standards for the Modern Calculator project, built with wxWidgets and C++17/20.

---

## 📁 Project Structure

```
project/
├── src/
│   ├── core/           # Core application logic
│   │   ├── app.h
│   │   └── app.cpp
│   └── ui/             # User interface
│       ├── main_window.h
│       ├── main_window.cpp
│       ├── button_panel.h
│       └── button_panel.cpp
└── docs/
    └── CODE_STYLE_GUIDE.md
```

---

## 🏗️ Naming Conventions

### Classes and Structures
- **PascalCase** for class and structure names
```cpp
class MainWindow : public wxFrame { };
struct ButtonInfo { };
```

### Methods and Functions
- **PascalCase** for public methods
- **PascalCase** for private methods
```cpp
public:
    void UpdateDisplay(const wxString& value);    // ✅
    void SetStatusMessage(const wxString& msg);   // ✅

private:
    void CreateUI();                              // ✅
    void SetupEventHandlers();                    // ✅
```

### Variables
- **camelCase** for local variables and parameters
- **m_** prefix + **camelCase** for class members
- **SCREAMING_SNAKE_CASE** for constants
```cpp
// Local variables
wxString currentNumber = "0";                     // ✅
bool isDarkTheme = false;                         // ✅

// Class members
wxTextCtrl* m_display;                           // ✅
std::unique_ptr<ButtonPanel> m_buttonPanel;     // ✅
bool m_waitingForOperand;                        // ✅

// Constants
static constexpr int MIN_WIDTH = 320;           // ✅
static constexpr int BUTTON_HEIGHT = 50;        // ✅
```

### Constants and Enumerations
```cpp
// Enumerations
enum IDs
{
    ID_ABOUT = wxID_ABOUT,
    ID_EXIT = wxID_EXIT,
    ID_THEME_TOGGLE = 2000
};

// Color constants
static const inline wxColour COLOR_NUMBER{ 240, 240, 240 };
static const inline wxColour COLOR_OPERATOR{ 255, 149, 0 };
```

---

## 🎨 Comment Style

### ASCII Comments for Header Files

#### Main Sections (Double Border)
```cpp
/*
 ╔═══════════════════════════════════════════════════════════════════════════╗
 ║                       🏠 CALCULATOR MAIN WINDOW                          ║
 ║                     Core user interface class                           ║
 ║                         for the application                             ║
 ╚═══════════════════════════════════════════════════════════════════════════╝
*/
```

#### Subsections (Single Lines)
```cpp
//──────────────────────────────────────────────────────────────────────────
// 🏗️ Constructor and Destructor
//──────────────────────────────────────────────────────────────────────────
```

#### Inline Comments
```cpp
void UpdateDisplay(const wxString& value);        // 📺 Update display
void SetStatusMessage(const wxString& message);   // 📝 Status message
bool m_isDarkTheme;                               // 🌙 Dark theme flag
```

### Comments in .cpp Files

#### Block Comments
```cpp
//==============================================================================
// USER INTERFACE CREATION
//==============================================================================

void MainWindow::CreateUI()
{
    // Create main panel
    m_mainPanel = new wxPanel(this, wxID_ANY);
    // ...
}
```

#### Functional Comments
```cpp
// Handle number input
void MainWindow::OnNumber(wxCommandEvent& event)
{
    wxString number = event.GetString();
    
    // If waiting for new operand
    if (m_waitingForOperand) {
        m_currentNumber = number;
        m_waitingForOperand = false;
    }
    // ...
}
```

---

## 🔧 Code Formatting

### Indentation and Spacing
- **4 spaces** for indentation (no tabs)
- Space after keywords: `if (`, `for (`, `while (`
- Spaces around operators: `a = b + c`

### Braces
- **Allman style** - braces on new line
```cpp
void MainWindow::CreateUI()
{
    if (condition)
    {
        // code
    }
    else
    {
        // code
    }
}
```

### Line Length
- **Maximum 100 characters** per line
- When exceeded - wrap with indentation

```cpp
// Long parameters
explicit ButtonPanel(wxWindow* parent,
    wxWindowID id = wxID_ANY,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize);

// Long function calls
m_display = new wxTextCtrl(m_mainPanel, wxID_ANY, "0",
    wxDefaultPosition, wxSize(-1, DISPLAY_HEIGHT),
    wxTE_RIGHT | wxTE_READONLY | wxBORDER_NONE);
```

---

## 🚀 Modern C++ Features

### Using Smart Pointers
```cpp
// ✅ Preferred
std::unique_ptr<ButtonPanel> m_buttonPanel;
std::array<std::unique_ptr<wxButton>, 10> m_numberButtons;

// ❌ Avoid
ButtonPanel* m_buttonPanel;
```

### Automatic Type Deduction
```cpp
// ✅ For complex types
auto* mainSizer = new wxBoxSizer(wxVERTICAL);
auto& buttonMap = m_operatorButtons;

// ❌ For simple types
auto x = 5;        // Better: int x = 5;
auto flag = true;  // Better: bool flag = true;
```

### Deleting Copy/Move Methods
```cpp
class MainWindow : public wxFrame
{
public:
    // Explicitly delete unwanted operations
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;
};
```

### Using constexpr
```cpp
// Compile-time constants
static constexpr int MIN_WIDTH = 320;
static constexpr int DISPLAY_HEIGHT = 60;
```

---

## 📋 Class Structure

### Class Element Order
1. **public** section
   - Constructors/destructor
   - Public methods
2. **private** section  
   - Private methods
   - Class members
   - Constants

### Method Grouping
```cpp
class MainWindow : public wxFrame
{
public:
    //──────────────────────────────────────────────────────────────────────────
    // 🏗️ Constructor and Destructor
    //──────────────────────────────────────────────────────────────────────────
    explicit MainWindow(/* parameters */);
    virtual ~MainWindow() = default;

    //──────────────────────────────────────────────────────────────────────────
    // 🎮 Public Interface
    //──────────────────────────────────────────────────────────────────────────
    void UpdateDisplay(const wxString& value);
    void SetStatusMessage(const wxString& message);

private:
    //──────────────────────────────────────────────────────────────────────────
    // ⚡ Event Handlers
    //──────────────────────────────────────────────────────────────────────────
    void OnNumber(wxCommandEvent& event);
    void OnOperator(wxCommandEvent& event);

    //──────────────────────────────────────────────────────────────────────────
    // 💾 Class Members
    //──────────────────────────────────────────────────────────────────────────
    std::unique_ptr<ButtonPanel> m_buttonPanel;
    wxTextCtrl* m_display;
};
```

---

## 🔗 wxWidgets Event Handling

### Event Binding
```cpp
// Custom events
Bind(EVT_CALC_NUMBER, &MainWindow::OnNumber, this);
Bind(EVT_CALC_OPERATOR, &MainWindow::OnOperator, this);

// System events
Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
Bind(wxEVT_KEY_DOWN, &MainWindow::OnKeyDown, this);
```

### Custom Event Declaration
```cpp
// In header file
wxDECLARE_EVENT(EVT_CALC_NUMBER, wxCommandEvent);
wxDECLARE_EVENT(EVT_CALC_OPERATOR, wxCommandEvent);

// In .cpp file
wxDEFINE_EVENT(EVT_CALC_NUMBER, wxCommandEvent);
wxDEFINE_EVENT(EVT_CALC_OPERATOR, wxCommandEvent);
```

---

## 🛡️ Error Handling

### Pointer Validation
```cpp
void MainWindow::UpdateDisplay(const wxString& value)
{
    if (m_display) {
        m_display->SetValue(value);
    }
}
```

### Mathematical Error Handling
```cpp
void MainWindow::OnEquals(wxCommandEvent& event)
{
    // Check for division by zero
    if (m_currentOperator == "/" && curr == 0.0) {
        SetDisplayError("Division by zero");
        return;
    }
    
    // Validate number conversion
    if (!m_previousNumber.ToDouble(&prev) || !m_currentNumber.ToDouble(&curr)) {
        SetDisplayError("Error");
        return;
    }
}
```

---

## 📦 Include Directives

### Include Order
1. Project-specific headers
2. wxWidgets headers
3. C++ standard library
4. System headers

```cpp
#include "ui/main_window.h"        // Project headers
#include "ui/button_panel.h"

#include <wx/wx.h>                 // wxWidgets
#include <wx/msgdlg.h>

#include <memory>                  // STL
#include <string>
#include <array>
#include <unordered_map>
```

---

## ✨ Additional Recommendations

### Using const
- Always use `const` for parameters that don't change
- Mark methods as `const` if they don't modify object state

```cpp
void UpdateDisplay(const wxString& value);              // ✅
wxColour GetButtonColor(const wxString& type) const;    // ✅
```

### Class Member Initialization
- Use initialization lists in constructors
- Initialize all class members

```cpp
MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title)
    : wxFrame(parent, id, title)
    , m_display(nullptr)
    , m_mainPanel(nullptr)
    , m_isDarkTheme(false)
    , m_waitingForOperand(true)
{
    // Constructor body
}
```

### Virtual Destructors
- Always declare destructors virtual for base classes

```cpp
class MainWindow : public wxFrame
{
public:
    virtual ~MainWindow() = default;  // ✅
};
```

---

## 🎯 Conclusion

This guide ensures consistency and readability of the project code. All development team members should follow these rules to maintain high code quality standards.

**Version:** 1.0  
**Last Updated:** August 2025  
**Author:** Calculator Development Team