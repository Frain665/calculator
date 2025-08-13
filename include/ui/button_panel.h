#ifndef BUTTON_PANEL_H
#define BUTTON_PANEL_H


#include <wx/wx.h>

#include <array>
#include <memory>
#include <unordered_map>
#include <string_view>

/*
 ╔═══════════════════════════════════════════════════════════════════════════╗
 ║                         📡 ПОЛЬЗОВАТЕЛЬСКИЕ СОБЫТИЯ                       ║
 ║                     Кастомные события для калькулятора                    ║
 ╚═══════════════════════════════════════════════════════════════════════════╝
*/
wxDECLARE_EVENT(EVT_CALC_NUMBER, wxCommandEvent);      // 🔢 Ввод числа
wxDECLARE_EVENT(EVT_CALC_OPERATOR, wxCommandEvent);    // ➕ Математический оператор
wxDECLARE_EVENT(EVT_CALC_EQUALS, wxCommandEvent);      // 🟰 Вычисление результата
wxDECLARE_EVENT(EVT_CALC_CLEAR, wxCommandEvent);       // 🗑️ Полная очистка
wxDECLARE_EVENT(EVT_CALC_CLEAR_ENTRY, wxCommandEvent); // ❌ Очистка текущего ввода
wxDECLARE_EVENT(EVT_CALC_DECIMAL, wxCommandEvent);     // • Десятичная точка
wxDECLARE_EVENT(EVT_CALC_BACKSPACE, wxCommandEvent);   // ⌫ Удаление символа

/*
 ╔═══════════════════════════════════════════════════════════════════════════╗
 ║                           🎛️ ПАНЕЛЬ КНОПОК                                ║
 ║          Основной класс для управления кнопками калькулятора              ║
 ║                                                                           ║
 ║  📊 Функциональность:                                                     ║
 ║   • Создание и управление кнопками                                        ║
 ║   • Обработка событий нажатий                                             ║
 ║   • Визуальные эффекты и подсветка                                        ║
 ║   • Адаптивная раскладка                                                  ║
 ╚═══════════════════════════════════════════════════════════════════════════╝
*/
class ButtonPanel : public wxPanel
{
public:
    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                         🏗️ КОНСТРУКТОРЫ И ДЕСТРУКТОР                   │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    explicit ButtonPanel(wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize);

    virtual ~ButtonPanel() = default;

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                        🚫 ЗАПРЕТ КОПИРОВАНИЯ                            │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    ButtonPanel(const ButtonPanel&) = delete;
    ButtonPanel& operator=(const ButtonPanel&) = delete;

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                        ↔️ РАЗРЕШЕНИЕ ПЕРЕМЕЩЕНИЯ                        │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    ButtonPanel(ButtonPanel&&) = default;
    ButtonPanel& operator=(ButtonPanel&&) = default;

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                         🎮 ПУБЛИЧНЫЕ МЕТОДЫ                             │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    /// 🔘 Включение/отключение кнопки по метке
    void SetButtonEnabled(const wxString& label, bool enabled);
    
    /// ✨ Подсветка операторной кнопки
    void HighlightOperator(const wxString& op, bool highlight = true);
    
    /// 💡 Сброс всех подсветок
    void ClearHighlights();

private:
    /*
     ╔═══════════════════════════════════════════════════════════════════════╗
     ║                          📏 КОНСТАНТЫ РАЗМЕРОВ                       ║
     ╚═══════════════════════════════════════════════════════════════════════╝
    */
    static constexpr int BUTTON_WIDTH = 70;     // 📐 Ширина кнопки
    static constexpr int BUTTON_HEIGHT = 50;    // 📏 Высота кнопки
    static constexpr int BUTTON_SPACING = 5;    // 📊 Интервал между кнопками
    static constexpr int PANEL_MARGIN = 10;     // 🖼️ Отступы панели

    /*
     ╔═══════════════════════════════════════════════════════════════════════╗
     ║                         📝 СТРУКТУРА КНОПКИ                          ║
     ║                    Содержит всю информацию о кнопке                   ║
     ╚═══════════════════════════════════════════════════════════════════════╝
    */
    struct ButtonInfo {
        wxString label;              // 🏷️ Текст на кнопке
        wxString value;              // 💾 Значение для передачи
        wxColour normalColor;        // 🎨 Обычный цвет
        wxColour highlightColor;     // ✨ Цвет при подсветке
        bool isOperator = false;     // ⚡ Флаг операторной кнопки

        ButtonInfo(const wxString& lbl, const wxString& val,
            const wxColour& normalCol, const wxColour& highlightCol = wxNullColour,
            bool op = false)
            : label(lbl), value(val), normalColor(normalCol), highlightColor(highlightCol), isOperator(op) {
        }
    };

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                      🔧 МЕТОДЫ ИНИЦИАЛИЗАЦИИ                            │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    void InitializeComponents();    // 🏗️ Основная инициализация
    void CreateButtons();          // 🎛️ Создание всех кнопок
    void SetupLayout();           // 📐 Настройка расположения
    void BindEvents();            // 🔗 Привязка событий

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                       🎨 МЕТОДЫ СТИЛИЗАЦИИ                              │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    /// 🏭 Фабрика стилизованных кнопок
    std::unique_ptr<wxButton> CreateStyledButton(const ButtonInfo& info);
    
    /// 🖌️ Применение стиля к кнопке
    void StyleButton(wxButton* button, const wxColour& bgColor);

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                      🎯 ОБРАБОТЧИКИ СОБЫТИЙ                             │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    void OnNumberClick(wxCommandEvent& event);      // 🔢 Нажатие цифры
    void OnOperatorClick(wxCommandEvent& event);    // ➕ Нажатие оператора
    void OnEqualsClick(wxCommandEvent& event);      // 🟰 Нажатие равно
    void OnClearClick(wxCommandEvent& event);       // 🗑️ Полная очистка
    void OnClearEntryClick(wxCommandEvent& event);  // ❌ Очистка ввода
    void OnDecimalClick(wxCommandEvent& event);     // • Десятичная точка
    void OnBackspaceClick(wxCommandEvent& event);   // ⌫ Удаление символа

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                       🛠️ УТИЛИТЫ                                        │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    /// 📡 Отправка пользовательского события
    void SendCustomEvent(wxEventType eventType, const wxString& data = wxEmptyString);
    
    /// 🎨 Получение цвета кнопки по типу
    wxColour GetButtonColor(const wxString& buttonType) const;
    
    /// 📐 Обработка изменения размера
    void OnSize(wxSizeEvent& event);

    /*
     ╔═══════════════════════════════════════════════════════════════════════╗
     ║                         💾 ЧЛЕНЫ КЛАССА                              ║
     ╚═══════════════════════════════════════════════════════════════════════╝
    */
    
    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                        🔢 КОНТЕЙНЕРЫ КНОПОК                             │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    std::array<std::unique_ptr<wxButton>, 10> m_numberButtons;              // 0-9
    std::unordered_map<wxString, std::unique_ptr<wxButton>> m_operatorButtons;  // +-*/
    std::unordered_map<wxString, std::unique_ptr<wxButton>> m_functionButtons;  // C, CE, =, etc.

    //┌─────────────────────────────────────────────────────────────────────────┐
    //│                        📋 ВСПОМОГАТЕЛЬНЫЕ ДАННЫЕ                        │
    //└─────────────────────────────────────────────────────────────────────────┘
    
    std::unordered_map<int, wxString> m_buttonValues;  // 🔗 ID -> значение кнопки
    std::unique_ptr<wxGridSizer> m_mainSizer;          // 📐 Главный компоновщик

    /*
     ╔═══════════════════════════════════════════════════════════════════════╗
     ║                          🎨 ЦВЕТОВАЯ ПАЛИТРА                         ║
     ║                      Статические константы цветов                    ║
     ╚═══════════════════════════════════════════════════════════════════════╝
    */
    
    static const inline wxColour COLOR_NUMBER{ 240, 240, 240 };           // 🔘 Цифры
    static const inline wxColour COLOR_OPERATOR{ 255, 149, 0 };           // ➕ Операторы
    static const inline wxColour COLOR_OPERATOR_HIGHLIGHT{ 255, 179, 64 }; // ✨ Подсветка операторов
    static const inline wxColour COLOR_FUNCTION{ 169, 169, 169 };         // ⚙️ Функциональные
    static const inline wxColour COLOR_EQUALS{ 255, 149, 0 };             // 🟰 Равно
    static const inline wxColour COLOR_CLEAR{ 255, 59, 48 };              // 🗑️ Очистка
    static const inline wxColour COLOR_TEXT{ 0, 0, 0 };                   // 🖤 Текст черный
    static const inline wxColour COLOR_TEXT_WHITE{ 255, 255, 255 };       // 🤍 Текст белый
};

/*
 ╔═══════════════════════════════════════════════════════════════════════════╗
 ║                        🔧 ШАБЛОННАЯ УТИЛИТА                               ║
 ║            Универсальная привязка событий для кнопок                      ║
 ╚═══════════════════════════════════════════════════════════════════════════╝
*/
template<typename Handler>
void BindButtonEvent(wxButton* button, Handler&& handler) 
{
    if (button) 
    {
        button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, std::forward<Handler>(handler));
    }
}

#endif //BUTTON_PANEL_H