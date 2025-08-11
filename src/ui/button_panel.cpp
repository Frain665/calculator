#include "ui/button_panel.h"
#include <algorithm>

wxDEFINE_EVENT(EVT_CALC_NUMBER, wxCommandEvent);
wxDEFINE_EVENT(EVT_CALC_OPERATOR, wxCommandEvent);
wxDEFINE_EVENT(EVT_CALC_EQUALS, wxCommandEvent);
wxDEFINE_EVENT(EVT_CALC_CLEAR, wxCommandEvent);
wxDEFINE_EVENT(EVT_CALC_CLEAR_ENTRY, wxCommandEvent);
wxDEFINE_EVENT(EVT_CALC_DECIMAL, wxCommandEvent);
wxDEFINE_EVENT(EVT_CALC_BACKSPACE, wxCommandEvent);

ButtonPanel::ButtonPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, id, pos, size)
{
    SetBackgroundColour(wxColour(50, 50, 50));
    InitializeComponents();
}

void ButtonPanel::InitializeComponents()
{
    CreateButtons();
    SetupLayout();
    BindEvents();
}

void ButtonPanel::CreateButtons()
{
    for (int i = 0; i <= 9; ++i) 
    {
        ButtonInfo info{ wxString::Format("%d", i), wxString::Format("%d", i), COLOR_NUMBER };
        m_numberButtons[i] = CreateStyledButton(info);
        m_buttonValues[m_numberButtons[i]->GetId()] = info.value;
    }

    const std::array<std::pair<wxString, wxString>, 4> operators = 
    { {
        {"+", "+"}, {"-", "-"}, {"×", "*"}, {"÷", "/"}
    } };

    for (const auto& [label, value] : operators) 
    {
        ButtonInfo info{ label, value, COLOR_OPERATOR, COLOR_OPERATOR_HIGHLIGHT, true };
        m_operatorButtons[label] = CreateStyledButton(info);
        m_buttonValues[m_operatorButtons[label]->GetId()] = info.value;
    }

    const std::array<std::tuple<wxString, wxString, wxColour>, 5> functions = { {
        {"C", "clear", COLOR_CLEAR},
        {"CE", "clear_entry", COLOR_FUNCTION},
        {"<-", "backspace", COLOR_FUNCTION},
        {".", ".", COLOR_NUMBER},
        {"=", "equals", COLOR_EQUALS}
    } };

    for (const auto& [label, value, color] : functions)
    {
        ButtonInfo info{ label, value, color };
        m_functionButtons[label] = CreateStyledButton(info);
        m_buttonValues[m_functionButtons[label]->GetId()] = info.value;
    }
}

std::unique_ptr<wxButton> ButtonPanel::CreateStyledButton(const ButtonInfo& info)
{
    auto button = std::make_unique<wxButton>(
        this,
        wxID_ANY,
        info.label,
        wxDefaultPosition,
        wxSize(BUTTON_WIDTH, BUTTON_HEIGHT)
    );

    StyleButton(button.get(), info.normalColor);
    return button;
}

void ButtonPanel::StyleButton(wxButton* button, const wxColour& bgColor)
{
    if (!button) return;

    button->SetBackgroundColour(bgColor);

    const bool isDark = (bgColor.Red() + bgColor.Green() + bgColor.Blue()) < 400;
    button->SetForegroundColour(isDark ? COLOR_TEXT_WHITE : COLOR_TEXT);

    wxFont font = button->GetFont();
    font.SetPointSize(14);
    font.SetWeight(wxFONTWEIGHT_SEMIBOLD);
    button->SetFont(font);

    button->SetCanFocus(false);
}

void ButtonPanel::SetupLayout()
{
    m_mainSizer = std::make_unique<wxGridSizer>(5, 4, BUTTON_SPACING, BUTTON_SPACING);

    m_mainSizer->Add(m_functionButtons["CE"].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_functionButtons["C"].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_functionButtons["<-"].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_operatorButtons["÷"].get(), 0, wxEXPAND);

    m_mainSizer->Add(m_numberButtons[7].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_numberButtons[8].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_numberButtons[9].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_operatorButtons["×"].get(), 0, wxEXPAND);

    m_mainSizer->Add(m_numberButtons[4].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_numberButtons[5].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_numberButtons[6].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_operatorButtons["-"].get(), 0, wxEXPAND);

    m_mainSizer->Add(m_numberButtons[1].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_numberButtons[2].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_numberButtons[3].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_operatorButtons["+"].get(), 0, wxEXPAND);

    m_mainSizer->Add(m_numberButtons[0].get(), 0, wxEXPAND);
    m_mainSizer->Add(new wxPanel(this), 0, wxEXPAND);
    m_mainSizer->Add(m_functionButtons["."].get(), 0, wxEXPAND);
    m_mainSizer->Add(m_functionButtons["="].get(), 0, wxEXPAND);

    auto* outerSizer = new wxBoxSizer(wxVERTICAL);
    outerSizer->Add(m_mainSizer.get(), 1, wxEXPAND | wxALL, PANEL_MARGIN);

    SetSizer(outerSizer);
}

void ButtonPanel::BindEvents()
{
    for (const auto& button : m_numberButtons) 
    {
        if (button) 
        {
            button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                &ButtonPanel::OnNumberClick, this);
        }
    }

    for (const auto& [label, button] : m_operatorButtons) 
    {
        if (button) 
        {
            button->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                &ButtonPanel::OnOperatorClick, this);
        }
    }

    if (m_functionButtons["C"]) 
    {
        m_functionButtons["C"]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
            &ButtonPanel::OnClearClick, this);
    }

    if (m_functionButtons["CE"]) 
    {
        m_functionButtons["CE"]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
            &ButtonPanel::OnClearEntryClick, this);
    }

    if (m_functionButtons["<-"]) 
    {
        m_functionButtons["<-"]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
            &ButtonPanel::OnBackspaceClick, this);
    }

    if (m_functionButtons["."]) 
    {
        m_functionButtons["."]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
            &ButtonPanel::OnDecimalClick, this);
    }

    if (m_functionButtons["="]) 
    {
        m_functionButtons["="]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
            &ButtonPanel::OnEqualsClick, this);
    }

    Bind(wxEVT_SIZE, &ButtonPanel::OnSize, this);
}

void ButtonPanel::OnNumberClick(wxCommandEvent& event)
{
    const auto it = m_buttonValues.find(event.GetId());
    if (it != m_buttonValues.end()) 
    {
        SendCustomEvent(EVT_CALC_NUMBER, it->second);
    }
}

void ButtonPanel::OnOperatorClick(wxCommandEvent& event)
{
    const auto it = m_buttonValues.find(event.GetId());
    if (it != m_buttonValues.end()) 
    {
        ClearHighlights();

        for (const auto& [label, button] : m_operatorButtons) 
        {
            if (button && button->GetId() == event.GetId()) 
            {
                HighlightOperator(label, true);
                break;
            }
        }

        SendCustomEvent(EVT_CALC_OPERATOR, it->second);
    }
}

void ButtonPanel::OnEqualsClick(wxCommandEvent& event)
{
    ClearHighlights();
    SendCustomEvent(EVT_CALC_EQUALS);
}

void ButtonPanel::OnClearClick(wxCommandEvent& event)
{
    ClearHighlights();
    SendCustomEvent(EVT_CALC_CLEAR);
}

void ButtonPanel::OnClearEntryClick(wxCommandEvent& event)
{
    SendCustomEvent(EVT_CALC_CLEAR_ENTRY);
}

void ButtonPanel::OnDecimalClick(wxCommandEvent& event)
{
    SendCustomEvent(EVT_CALC_DECIMAL, ".");
}

void ButtonPanel::OnBackspaceClick(wxCommandEvent& event)
{
    SendCustomEvent(EVT_CALC_BACKSPACE);
}

void ButtonPanel::SendCustomEvent(wxEventType eventType, const wxString& data)
{
    wxCommandEvent evt(eventType, GetId());
    evt.SetEventObject(this);
    evt.SetString(data);
    GetEventHandler()->ProcessEvent(evt);
}

void ButtonPanel::SetButtonEnabled(const wxString& label, bool enabled)
{
    for (const auto& button : m_numberButtons) 
    {
        if (button && button->GetLabel() == label) 
        {
            button->Enable(enabled);
            return;
        }
    }

    auto opIt = m_operatorButtons.find(label);
    if (opIt != m_operatorButtons.end() && opIt->second) 
    {
        opIt->second->Enable(enabled);
        return;
    }

    auto funcIt = m_functionButtons.find(label);
    if (funcIt != m_functionButtons.end() && funcIt->second) 
    {
        funcIt->second->Enable(enabled);
    }
}

void ButtonPanel::HighlightOperator(const wxString& op, bool highlight)
{
    auto it = m_operatorButtons.find(op);
    if (it != m_operatorButtons.end() && it->second) 
    {
        const wxColour color = highlight ? COLOR_OPERATOR_HIGHLIGHT : COLOR_OPERATOR;
        StyleButton(it->second.get(), color);
        it->second->Refresh();
    }
}

void ButtonPanel::ClearHighlights()
{
    for (const auto& [label, button] : m_operatorButtons) {
        if (button) 
        {
            StyleButton(button.get(), COLOR_OPERATOR);
            button->Refresh();
        }
    }
}

void ButtonPanel::OnSize(wxSizeEvent& event)
{
    Layout();
    event.Skip();
}