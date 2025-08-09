#include "gui/gui.hpp"

#include "raylib.h"

extern void Execute(const std::string &str);

Button::Button(const GuiInfo& info, const std::string& text, const std::string& onClick) : 
    m_OnClick(onClick), m_Text(info, text)
{
    m_Info = info;
}

void Button::OnUpdate() {
    Vector2 mouse = Gui.GetMousePos();
    Rectangle actual = Gui.GetRealSize(m_Info);

    // so you can hold down on a button and nothing happens until you let go
    // common practice from what i've seen
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && m_State == 2) {
        Execute(m_OnClick);
    }

    if (CheckCollisionPointRec(mouse, actual)) {
        m_State = 1; // Hovered
        Gui.m_Hovering = true;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            m_State = 2; // Clicked
        }
    } else {
        m_State = 0; // Default
    }

    m_Text.OnUpdate();
}

void Button::OnRender() {
    Rectangle actual = Gui.GetRealSize(m_Info);

    switch (m_State) {
        case 0:
            Gui.RenderRectangle(actual, m_Outline, m_Style.DefaultFill, m_Style.DefaultOutline);
            break;
        case 1:
            Gui.RenderRectangle(actual, m_Outline, m_Style.HoveredFill, m_Style.HoveredOutline);
            break;
        case 2:
            Gui.RenderRectangle(actual, m_Outline, m_Style.ClickedFill, m_Style.ClickedOutline);
            break;
    }

    m_Text.OnRender();
}
