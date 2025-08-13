# Contributing to Modern Calculator

Thank you for your interest in contributing to Modern Calculator! We welcome contributions from developers of all skill levels. This guide will help you get started.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Project Structure](#project-structure)
- [Testing Guidelines](#testing-guidelines)
- [Issue Guidelines](#issue-guidelines)

## 🤝 Code of Conduct

By participating in this project, you agree to abide by our Code of Conduct. Please be respectful, inclusive, and constructive in all interactions.

## 🚀 Getting Started

### Prerequisites

- C++17 or later compiler (GCC 7+, Clang 5+, MSVC 2019+)
- wxWidgets 3.1+ development libraries
- CMake 3.16 or later
- Git

### Development Setup

1. **Fork the repository**
   ```bash
   # Click the "Fork" button on GitHub, then clone your fork
   git clone https://github.com/Frain665/calculator.git
   cd modern-calculator
   ```

2. **Set up upstream remote**
   ```bash
   git remote add upstream https://github.com/Frain665/modern-calculator.git
   ```

3. **Install dependencies**
   
   **Ubuntu/Debian:**
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential cmake libwxgtk3.0-gtk3-dev
   ```
   
   **macOS:**
   ```bash
   brew install wxwidgets cmake
   ```
   
   **Windows:**
   - Install Visual Studio 2019/2022 with C++ tools
   - Download and build wxWidgets from source or use vcpkg

4. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   make  # or cmake --build . on Windows
   ```

## 💡 How to Contribute

### 🐛 Bug Reports

When filing bug reports, please include:

- Clear description of the issue
- Steps to reproduce the bug
- Expected vs actual behavior
- Operating system and compiler version
- wxWidgets version
- Code snippet (if applicable)

### ✨ Feature Requests

For feature requests, please provide:

- Clear description of the proposed feature
- Use case and motivation
- Possible implementation approach
- Mockups or examples (if UI-related)

### 🔧 Code Contributions

We welcome contributions in these areas:

**High Priority:**
- Implementation of theme system (`OnThemeToggle`, `SetDarkTheme`)
- Keyboard shortcuts (`OnKeyDown`)
- Menu bar functionality (`CreateMenuBar`)
- Fullscreen mode (`OnFullScreen`)
- Modern styling improvements (`ApplyModernStyle`)

**Medium Priority:**
- Scientific calculator functions
- Memory operations (M+, M-, MC, MR)
- Calculation history
- Expression parsing improvements

**Low Priority:**
- Unit tests
- Documentation improvements
- Code refactoring and optimization
- Additional themes

## 🔄 Pull Request Process

1. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b bugfix/issue-number
   ```

2. **Make your changes**
   - Follow the coding standards below
   - Test your changes thoroughly
   - Update documentation if needed

3. **Commit your changes**
   ```bash
   git add .
   git commit -m "feat: add dark theme support
   
   - Implement SetDarkTheme function
   - Add theme toggle menu item
   - Update UI colors for dark mode
   
   Closes #123"
   ```

4. **Push and create PR**
   ```bash
   git push origin feature/your-feature-name
   ```
   
   Then create a Pull Request on GitHub with:
   - Clear title and description
   - Reference to related issues
   - Screenshots (for UI changes)
   - Testing notes

## 📝 Coding Standards

### C++ Style Guidelines

- **Naming Conventions:**
  - Classes: `PascalCase` (e.g., `MainWindow`, `ButtonPanel`)
  - Methods: `PascalCase` (e.g., `OnNumber`, `SetupLayout`)
  - Variables: `camelCase` with `m_` prefix for members (e.g., `m_display`, `m_currentNumber`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MIN_WIDTH`, `DISPLAY_HEIGHT`)

- **Code Formatting:**
  - Use 4 spaces for indentation
  - Opening braces on same line for functions and classes
  - One line between method definitions
  - Maximum line length: 100 characters

- **Memory Management:**
  - Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) when possible
  - Follow RAII principles
  - Properly handle wxWidgets object lifecycle

### Example Code Style:

```cpp
void MainWindow::OnNumber(wxCommandEvent& event)
{
    wxString number = event.GetString();
    
    if (m_waitingForOperand) 
    {
        m_currentNumber = number;
        m_waitingForOperand = false;
    }
    else 
    {
        m_currentNumber += number;
    }
    
    UpdateDisplay(m_currentNumber);
}
```

## 📁 Project Structure

```
modern-calculator/
├── src/
│   ├── main.cpp              # Application entry point
│   └── ui/
│       ├── main_window.cpp   # Main window implementation
│       ├── main_window.h     # Main window header
│       └── button_panel.cpp  # Button panel implementation
├── include/                  # Public headers
├── tests/                    # Unit tests
├── docs/                     # Documentation
├── resources/               # Icons, images, etc.
└── CMakeLists.txt          # Build configuration
```

## 🧪 Testing Guidelines

- Write unit tests for new functionality
- Test on multiple platforms when possible
- Verify UI responsiveness at different window sizes
- Test keyboard shortcuts and edge cases
- Check memory leaks with valgrind (Linux/macOS)

### Running Tests

```bash
# In build directory
ctest --verbose
```

## 📋 Issue Guidelines

### Labels We Use

- `bug` - Something isn't working
- `enhancement` - New feature or request
- `good first issue` - Good for newcomers
- `help wanted` - Extra attention needed
- `priority: high/medium/low` - Issue priority
- `platform: windows/linux/macos` - Platform-specific issues

### Issue Templates

When creating issues, please use our issue templates and provide all requested information.

## 🆘 Getting Help

- **Email:** [steamrop001@gmail.com] for private matters

## 🙏 Recognition

All contributors will be recognized in our README.md file. Thank you for making Modern Calculator better!

---

## Quick Checklist for Contributors

- [ ] Code follows the project's style guidelines
- [ ] Self-review of the code has been performed
- [ ] Code has been tested on at least one platform
- [ ] Any dependent changes have been merged
- [ ] Documentation has been updated (if applicable)
- [ ] PR has a clear description and references related issues

**Happy coding! 🚀**