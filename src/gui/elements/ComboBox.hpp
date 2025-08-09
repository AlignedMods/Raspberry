#pragma once

#include "GuiElement.hpp"
#include "Text.hpp"

#include <vector>

class ComboBox : public GuiElement {
public:
    ComboBox(const GuiInfo& info, const std::vector<std::string>& options);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    sz m_Index = 0;
    std::vector<std::string> m_Options;

    u32 m_Outline = 2;

    Text m_Text;

    u32 m_State = 0;
};
