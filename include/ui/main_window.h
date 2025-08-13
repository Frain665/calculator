#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <wx/wx.h>
#include <memory>
#include <string>
#include "ui/button_panel.h"

/*
 ╔═══════════════════════════════════════════════════════════════════════════╗
 ║                          🏠 ГЛАВНОЕ ОКНО КАЛЬКУЛЯТОРА                    ║
 ║                      Основной класс пользовательского                    ║
 ║                          интерфейса приложения                           ║
 ╚═══════════════════════════════════════════════════════════════════════════╝
*/
class MainWindow : public wxFrame
{
public:
    //──────────────────────────────────────────────────────────────────────────
    // 🏗️ Конструктор и деструктор
    //──────────────────────────────────────────────────────────────────────────

    explicit MainWindow(wxWindow* parent = nullptr,
        wxWindowID id = wxID_ANY,
        const wxString& title = "Calculator",
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(350, 500));

    virtual ~MainWindow() = default;

    //──────────────────────────────────────────────────────────────────────────
    // 🚫 Запрет копирования и перемещения
    //──────────────────────────────────────────────────────────────────────────

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

    //──────────────────────────────────────────────────────────────────────────
    // 🎮 Публичный интерфейс
    //──────────────────────────────────────────────────────────────────────────

    void UpdateDisplay(const wxString& value);        // 📺 Обновление дисплея
    void SetStatusMessage(const wxString& message);   // 📝 Сообщение в статусе
    void ClearDisplay();                              // 🧹 Очистка дисплея, не реализована
    void SetDisplayError(const wxString& errorMsg);   // ❌ Отображение ошибки

private:
    //──────────────────────────────────────────────────────────────────────────
    // ⚡ Обработчики событий калькулятора
    //──────────────────────────────────────────────────────────────────────────

    void OnNumber(wxCommandEvent& event);      // 🔢 Ввод цифр
    void OnOperator(wxCommandEvent& event);    // ➕ Математические операторы
    void OnEquals(wxCommandEvent& event);      // 🟰 Вычисление результата
    void OnClear(wxCommandEvent& event);       // 🗑️ Полная очистка
    void OnClearEntry(wxCommandEvent& event);  // ❌ Очистка текущего ввода
    void OnDecimal(wxCommandEvent& event);     // • Десятичная точка
    void OnBackspace(wxCommandEvent& event);   // ⌫ Удаление символа

    //──────────────────────────────────────────────────────────────────────────
    // 🖼️ Обработчики системных событий
    //──────────────────────────────────────────────────────────────────────────

    void OnExit(wxCommandEvent& event);        // 🚪 Выход из приложения
    void OnAbout(wxCommandEvent& event);       // ℹ️ О программе
    void OnClose(wxCloseEvent& event);         // ❌ Закрытие окна
    void OnThemeToggle(wxCommandEvent& event); // 🎨 Переключение темы
    void OnFullScreen(wxCommandEvent& event);  // 📺 Полноэкранный режим
    void OnKeyDown(wxKeyEvent& event);         // ⌨️ Клавиатурный ввод
    void OnSize(wxSizeEvent& event);           // 📐 Изменение размера

    //──────────────────────────────────────────────────────────────────────────
    // 🔧 Методы инициализации
    //──────────────────────────────────────────────────────────────────────────

    void CreateMenuBar();          // 📋 Создание меню
    void CreateUI();               // 🎛️ Создание интерфейса
    void SetupLayout();            // 📐 Настройка компоновки
    void SetupEventHandlers();     // 🔗 Привязка событий
    void ApplyModernStyle();       // 🎨 Применение стилей
    void SetDarkTheme(bool dark = true); // 🌙 Темная тема

    //──────────────────────────────────────────────────────────────────────────
    // 💾 Компоненты интерфейса
    //──────────────────────────────────────────────────────────────────────────

    std::unique_ptr<ButtonPanel> m_buttonPanel; // 🎛️ Панель с кнопками
    wxTextCtrl* m_display;                      // 📺 Дисплей для чисел
    wxPanel* m_mainPanel;                       // 🖼️ Главная панель
    wxStaticText* m_statusLabel;                // 📊 Строка состояния

    //──────────────────────────────────────────────────────────────────────────
    // ⚙️ Состояние приложения
    //──────────────────────────────────────────────────────────────────────────

    bool m_isDarkTheme;         // 🌙 Флаг темной темы
    bool m_isFullscreen;        // 📺 Флаг полноэкранного режима

    //──────────────────────────────────────────────────────────────────────────
    // 🧮 Состояние калькулятора
    //──────────────────────────────────────────────────────────────────────────

    wxString m_currentNumber;    // 🔢 Текущее число
    wxString m_previousNumber;   // 📝 Предыдущее число
    wxString m_currentOperator;  // ➕ Текущий оператор
    bool m_waitingForOperand;    // ⏳ Ожидание операнда
    bool m_hasDecimal;           // • Наличие десятичной точки

    //──────────────────────────────────────────────────────────────────────────
    // 🆔 Идентификаторы элементов
    //──────────────────────────────────────────────────────────────────────────

    enum IDs
    {
        ID_ABOUT = wxID_ABOUT,
        ID_EXIT = wxID_EXIT,
        ID_THEME_TOGGLE = 2000,
        ID_FULLSCREEN = 2001
    };

    //──────────────────────────────────────────────────────────────────────────
    // 📏 Константы размеров
    //──────────────────────────────────────────────────────────────────────────

    static constexpr int MIN_WIDTH = 320;      // 📐 Минимальная ширина
    static constexpr int MIN_HEIGHT = 480;     // 📏 Минимальная высота
    static constexpr int DISPLAY_HEIGHT = 60;  // 📺 Высота дисплея
    static constexpr int STATUS_HEIGHT = 25;   // 📊 Высота статус-бара
};

#endif // MAIN_WINDOW_H