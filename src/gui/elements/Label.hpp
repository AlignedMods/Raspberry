#pragma once

#include "GuiElement.hpp"
#include "Text.hpp"

class Label : public GuiElement {
public:
    Label(const GuiInfo& info, const std::string& text);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    Text m_Text;
};
