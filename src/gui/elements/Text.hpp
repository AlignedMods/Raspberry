#pragma once

#include "GuiElement.hpp"

class Text : public GuiElement {
public:
    Text(const PosInfo& info, const std::string& text);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void ChangeText(const std::string& text);

private:
    std::string m_Text;
    f32 m_FontSize = 0.0f;
    f32 m_BaseFontSize = 100.0f;

    f32 m_TextWidth = 0.0f;
    f32 m_TextHeight = 0.0f;

    bool m_Calculated = false;

    Vector2 m_Padding = {1.2f, 1.2f};
};
