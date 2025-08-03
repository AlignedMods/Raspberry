#include <format>
#include "core/log.hpp"
#include "gui/gui.hpp"
#include "raylib.h"

Text::Text(const PosInfo& bounds, const std::string& text) :
    m_Text(text) 
{
    m_Info = bounds;
}

void Text::OnUpdate() {
    if (m_Text == "$NOTEXT") {
        return;
    }

    Rectangle actual = Gui.GetRealSize(m_Info);

    if (!m_Calculated) {
        if (!Gui.m_Fonts.contains(m_BaseFontSize)) {
            Gui.m_Fonts[m_BaseFontSize] = LoadFontEx("Assets/Fonts/alagard.ttf", m_BaseFontSize, nullptr, 0);
        }

        Vector2 size = MeasureTextEx(Gui.m_Fonts.at(m_BaseFontSize), m_Text.c_str(), m_BaseFontSize, 3);

        f32 scaleX = (actual.width) / (size.x * m_Padding.x);
        f32 scaleY = (actual.height) / (size.y * m_Padding.y);

        f32 scale = std::min(scaleX, scaleY);

        m_FontSize = m_BaseFontSize * scale;

        if (!Gui.m_Fonts.contains(m_FontSize)) {
            Gui.m_Fonts[m_FontSize] = LoadFontEx("Assets/Fonts/alagard.ttf", m_FontSize, nullptr, 0);
        }

        m_TextWidth = MeasureTextEx(Gui.m_Fonts.at(m_FontSize), m_Text.c_str(), m_FontSize, 3).x;
        m_TextHeight = MeasureTextEx(Gui.m_Fonts.at(m_FontSize), m_Text.c_str(), m_FontSize, 3).y;

        m_Calculated = true;
    }

}

void Text::OnRender() {
    if (m_Text == "$NOTEXT") {
        return;
    }

    Rectangle actual = Gui.GetRealSize(m_Info);

    if (!Gui.m_Fonts.contains(m_FontSize)) {
        Gui.m_Fonts[m_FontSize] = LoadFontEx("Assets/Fonts/alagard.ttf", m_FontSize, nullptr, 0);

        m_TextWidth = MeasureTextEx(Gui.m_Fonts.at(m_FontSize), m_Text.c_str(), m_FontSize, 3).x;
        m_TextHeight = MeasureTextEx(Gui.m_Fonts.at(m_FontSize), m_Text.c_str(), m_FontSize, 3).y;
    }

    DrawTextEx(Gui.m_Fonts.at(m_FontSize), m_Text.c_str(), {actual.x + actual.width / 2.0f - m_TextWidth / 2.0f, 
                                                            actual.y + actual.height / 2.0f - m_TextHeight / 2.0f}, m_FontSize, 3, GetColor(Colors::White));
    // DrawTextEx(Gui.m_Fonts.at(m_FontSize), m_Text.c_str(), {actual.x, actual.y}, m_FontSize, 3, GetColor(Colors::White));
}
