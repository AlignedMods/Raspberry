#include "ComboBox.hpp"
#include "gui/gui.hpp"

ComboBox::ComboBox(const PosInfo& info, const std::vector<std::string>& options) :
    m_Options(options), m_Text(info, options.at(0))
{
    m_Info = info;
}

void ComboBox::OnUpdate() {
    Rectangle actual = Gui.GetRealSize(m_Info);

    if (CheckCollisionPointRec(Gui.GetMousePos(), actual)) {
        m_State = 1;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { m_State = 2; }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_Index++;

            if (m_Index == m_Options.size()) { m_Index = 0; }
            if (m_Index > 100000000) { m_Index = m_Options.size() - 1; }

            m_Text.ChangeText(m_Options.at(m_Index));
        }
    } else {
        m_State = 0;
    }

    m_Text.OnUpdate();
}

void ComboBox::OnRender() {
    Rectangle actual = Gui.GetRealSize(m_Info);

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

    m_Text.OnRender();
}
