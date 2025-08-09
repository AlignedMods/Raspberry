#include "gui/gui.hpp"

Label::Label(const GuiInfo& info, const std::string& text) : 
    m_Text(info, text)
{
    m_Info = info;
}

void Label::OnUpdate() {
    m_Text.OnUpdate();
}

void Label::OnRender() {
    Rectangle actual = Gui.GetRealSize(m_Info);

    DrawRectangleRec(actual, GetColor(m_Style.DefaultFill));

    m_Text.OnRender();
}
