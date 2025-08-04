#include "Slider.hpp"
#include "gui/gui.hpp"

#include "raylib.h"

#include <cmath>

Slider::Slider(const PosInfo& info, f32 inital, f32 min, f32 max, f32 step) : 
    m_Value(inital), m_Min(min), m_Max(max), m_Step(step)
{
    m_Info = info;

    m_Normalized = (m_Value - m_Min) / (m_Max - m_Min);
}

void Slider::OnUpdate() {
    m_Normalized = (m_Value - m_Min) / (m_Max - m_Min);

    Rectangle actual = Gui.GetRealSize(m_Info);
    m_Slider = {actual.x + m_Normalized * (actual.width - 10), actual.y, 10, actual.height};

    Vector2 mousePos = Gui.GetMousePos();

    if (CheckCollisionPointRec(Gui.GetMousePos(), actual)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_State = 1;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        m_State = 0;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && m_State == 1) {
        m_Value = (m_Max - m_Min) * ((mousePos.x - actual.x - m_Slider.width / 2) / (actual.width - m_Slider.width)) + m_Min;

        m_Steps = std::round((m_Value - m_Min) / m_Step);
        m_Value = m_Min + m_Steps * m_Step;

        if (m_Value > m_Max) { m_Value = m_Max; }
        if (m_Value < m_Min) { m_Value = m_Min; }
    }
}

void Slider::OnRender() {
    Rectangle actual = Gui.GetRealSize(m_Info);

    Gui.RenderRectangle(actual, 2, m_Style.ClickedFill, m_Style.ClickedOutline);

    Gui.RenderRectangle(m_Slider, 2, m_Style.DefaultFill, m_Style.DefaultOutline);
}
