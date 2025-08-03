#include "core/log.hpp"
#include "gui/gui.hpp"

extern void Execute(const std::string &str);

Button::Button(const PosInfo& info, const std::string& text, const std::string& onClick) : 
    m_OnClick(onClick), m_Text(info, text)
{
    m_Bounds = info;
}

void Button::OnUpdate() {
    Vector2 mouse = Gui.GetMousePos();
    Rectangle actual = Gui.GetRealSize(m_Bounds);

    if (CheckCollisionPointRec(mouse, actual)) {
        m_State = 1; // Hovered
        Gui.m_Hovering = true;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            m_State = 2; // Clicked
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Execute(m_OnClick);
        }
    } else {
        m_State = 0; // Default
    }

    m_Text.OnUpdate();
}

void Button::OnRender() {
    Rectangle actual = Gui.GetRealSize(m_Bounds);

    switch (m_State) {
        case 0:
            DrawRectangleRec(actual, GetColor(m_Style.DefaultFill));
            break;
        case 1:
            DrawRectangleRec(actual, GetColor(m_Style.HoveredFill));
            break;
        case 2:
            DrawRectangleRec(actual, GetColor(m_Style.ClickedFill));
            break;
    }

    m_Text.OnRender();
}
