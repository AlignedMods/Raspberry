#pragma once

#include "GuiElement.hpp"

class Slider : public GuiElement {
public:
    Slider(const PosInfo& info, f32 inital, f32 min, f32 max, f32 step);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    Rectangle m_Slider;

    f32 m_Value = 0.0f;

    f32 m_Min = 0.0f;
    f32 m_Max = 32.0f;
    f32 m_Step = 1.0f;

    f32 m_Steps = 0.0f;

    f32 m_Normalized = 0.0f;

    u32 m_State = 0;
};
