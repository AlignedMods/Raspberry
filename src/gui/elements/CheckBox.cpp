#include "gui/gui.hpp"
#include "raylib.h"

CheckBox::CheckBox(const GuiInfo& info) {
    m_Info = info;
};

void CheckBox::OnUpdate() {
    Vector2 mouse = Gui.GetMousePos();
    Rectangle actual = Gui.GetRealSize(m_Info);

    // so you can hold down on a button and nothing happens until you let go
    // common practice from what i've seen
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && m_State == 2) {
        m_On = !m_On;
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
}

void CheckBox::OnRender() {
    Rectangle inner;

    Rectangle actual = Gui.GetRealSize(m_Info);

    if (m_On) {
        inner = {actual.x + 4 * Gui.GetScale(), actual.y + 4 * Gui.GetScale(), actual.width - 8 * Gui.GetScale(), actual.height - 8 * Gui.GetScale()};
    }

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

    if (m_On) {
        Gui.RenderRectangle(inner, 0, m_Style.DefaultOutline, m_Style.DefaultOutline);
    }
}
