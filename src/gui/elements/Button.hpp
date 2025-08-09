#pragma once

#include "GuiElement.hpp"
#include "Text.hpp"

class Button : public GuiElement {
public:
    Button(const GuiInfo& info, const std::string& text, const std::string& onClick);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    std::string m_OnClick;
    Text m_Text;
    
    u32 m_Outline = 2;

    u32 m_State = 0;
};
