#pragma once

#include "GuiElement.hpp"
#include "core/util.hpp"

class ColorPicker : public GuiElement {
public:
    ColorPicker(const GuiInfo& info);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    Vector3 m_Color = {360, 1, 1};

    Vector2 m_Picker;
    Vector2 m_HuePicker;

    Rectangle m_HueBar;
    Rectangle m_ColorPicker;

    u32 m_State = 0;

    f32 s = 0.0f;
};
