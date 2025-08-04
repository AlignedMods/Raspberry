#pragma once

#include "GuiElement.hpp"

class CheckBox : public GuiElement {
public:
    CheckBox(const PosInfo& info);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    u32 m_State = 0;
    u32 m_Outline = 2;

    bool m_On = false;
};
