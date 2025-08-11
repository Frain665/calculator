#pragma once

#include <wx/wx.h>
#include <array>
#include <memory>
#include <unordered_map>
#include <string_view>

// ѕользовательские событи€ дл€ калькул€тора
wxDECLARE_EVENT(EVT_CALC_NUMBER, wxCommandEvent);
wxDECLARE_EVENT(EVT_CALC_OPERATOR, wxCommandEvent);
wxDECLARE_EVENT(EVT_CALC_EQUALS, wxCommandEvent);
wxDECLARE_EVENT(EVT_CALC_CLEAR, wxCommandEvent);
wxDECLARE_EVENT(EVT_CALC_CLEAR_ENTRY, wxCommandEvent);
wxDECLARE_EVENT(EVT_CALC_DECIMAL, wxCommandEvent);
wxDECLARE_EVENT(EVT_CALC_BACKSPACE, wxCommandEvent);

class ButtonPanel : public wxPanel
{
public:
    explicit ButtonPanel(wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize);

    virtual ~ButtonPanel() = default;

    ButtonPanel(const ButtonPanel&) = delete;
    ButtonPanel& operator=(const ButtonPanel&) = delete;

    ButtonPanel(ButtonPanel&&) = default;
    ButtonPanel& operator=(ButtonPanel&&) = default;

    void SetButtonEnabled(const wxString& label, bool enabled);
    void HighlightOperator(const wxString& op, bool highlight = true);
    void ClearHighlights();

private:
    static constexpr int BUTTON_WIDTH = 70;
    static constexpr int BUTTON_HEIGHT = 50;
    static constexpr int BUTTON_SPACING = 5;
    static constexpr int PANEL_MARGIN = 10;

    struct ButtonInfo {
        wxString label;
        wxString value;
        wxColour normalColor;
        wxColour highlightColor;
        bool isOperator = false;

        ButtonInfo(const wxString& lbl, const wxString& val,
            const wxColour& normalCol, const wxColour& highlightCol = wxNullColour,
            bool op = false)
            : label(lbl), value(val), normalColor(normalCol), highlightColor(highlightCol), isOperator(op) {
        }
    };

    void InitializeComponents();
    void CreateButtons();
    void SetupLayout();
    void BindEvents();

    std::unique_ptr<wxButton> CreateStyledButton(const ButtonInfo& info);
    void StyleButton(wxButton* button, const wxColour& bgColor);

    void OnNumberClick(wxCommandEvent& event);
    void OnOperatorClick(wxCommandEvent& event);
    void OnEqualsClick(wxCommandEvent& event);
    void OnClearClick(wxCommandEvent& event);
    void OnClearEntryClick(wxCommandEvent& event);
    void OnDecimalClick(wxCommandEvent& event);
    void OnBackspaceClick(wxCommandEvent& event);

    void SendCustomEvent(wxEventType eventType, const wxString& data = wxEmptyString);
    wxColour GetButtonColor(const wxString& buttonType) const;

    void OnSize(wxSizeEvent& event);

    std::array<std::unique_ptr<wxButton>, 10> m_numberButtons;
    std::unordered_map<wxString, std::unique_ptr<wxButton>> m_operatorButtons;
    std::unordered_map<wxString, std::unique_ptr<wxButton>> m_functionButtons;

    std::unordered_map<int, wxString> m_buttonValues;

    std::unique_ptr<wxGridSizer> m_mainSizer;

    static const inline wxColour COLOR_NUMBER{ 240, 240, 240 };
    static const inline wxColour COLOR_OPERATOR{ 255, 149, 0 };
    static const inline wxColour COLOR_OPERATOR_HIGHLIGHT{ 255, 179, 64 };
    static const inline wxColour COLOR_FUNCTION{ 169, 169, 169 };
    static const inline wxColour COLOR_EQUALS{ 255, 149, 0 };
    static const inline wxColour COLOR_CLEAR{ 255, 59, 48 };
    static const inline wxColour COLOR_TEXT{ 0, 0, 0 };
    static const inline wxColour COLOR_TEXT_WHITE{ 255, 255, 255 };
};

template<typename Handler>
void BindButtonEvent(wxButton* button, Handler&& handler) 
{
    if (button) 
    {
        button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, std::forward<Handler>(handler));
    }
}