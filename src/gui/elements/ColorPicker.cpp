#include "ColorPicker.hpp"
#include <cmath>
#include <format>
#include "core/log.hpp"
#include "gui/gui.hpp"
#include "raylib.h"

ColorPicker::ColorPicker(const GuiInfo& info) {
    m_Info = info;

    m_ColorPicker = Gui.GetRealSize(m_Info);

    m_Picker.x = m_ColorPicker.x;
    m_Picker.y = m_ColorPicker.y;

    m_Color.y = 0;
    m_Color.z = 0;

    m_HuePicker.y = m_ColorPicker.y;
}

void ColorPicker::OnUpdate() {
    m_ColorPicker = Gui.GetRealSize(m_Info);

    m_ColorPicker.width = m_ColorPicker.width - 30.0f * Gui.GetScale();
    m_HueBar = {m_ColorPicker.x + m_ColorPicker.width + 10.0f * Gui.GetScale(), m_ColorPicker.y, 20.0f * Gui.GetScale(), m_ColorPicker.height};

    m_HuePicker.x = m_HueBar.x;

    Vector2 mouse = Gui.GetMousePos();

    if (CheckCollisionPointRec(mouse, m_ColorPicker)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_State = 1;
        }
    }

    if (CheckCollisionPointRec(mouse, m_HueBar)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_State = 2;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && (m_State == 1 || m_State == 2)) {
        m_State = 0;
    }

    // color panel
    if (m_State == 1) {
        m_Picker.x = mouse.x;
        m_Picker.y = mouse.y;

        if (m_Picker.x < m_ColorPicker.x) { m_Picker.x = m_ColorPicker.x; }
        if (m_Picker.x > m_ColorPicker.x + m_ColorPicker.width) { m_Picker.x = m_ColorPicker.x + m_ColorPicker.width; }

        if (m_Picker.y < m_ColorPicker.y) { m_Picker.y = m_ColorPicker.y; }
        if (m_Picker.y > m_ColorPicker.y + m_ColorPicker.height) { m_Picker.y = m_ColorPicker.y + m_ColorPicker.height; }

        m_Color.z = 1.0f - ((m_Picker.y - m_ColorPicker.y) / m_ColorPicker.height);
        m_Color.y = (m_Picker.x - m_ColorPicker.x) / m_ColorPicker.width;
    }

    // huebar
    if (m_State == 2) {
        m_HuePicker.y = mouse.y;

        if (m_HuePicker.y < m_HueBar.y) { m_HuePicker.y = m_HueBar.y; }
        if (m_HuePicker.y > m_HueBar.y + m_HueBar.height) { m_HuePicker.y = m_HueBar.y + m_HueBar.height; }

        m_Color.x = (mouse.y - m_HueBar.y) * 360.0f / m_HueBar.height;

        if (m_Color.x <= 0.0f) { m_Color.x = 0.0f; }
        if (m_Color.x >= 359.0f) { m_Color.x = 359.0f; }
    }
}

void ColorPicker::OnRender() {
    Vector3 rgbHue = ConvertHSVtoRGB({m_Color.x, 1.0f, 1.0f});
    Color maxHue = { (u8)(255.0f*rgbHue.x),
                     (u8)(255.0f*rgbHue.y),
                     (u8)(255.0f*rgbHue.z), 255};

    // hue slider bar
    DrawRectangleGradientV(m_HueBar.x, m_HueBar.y,                         m_HueBar.width, std::ceil(m_HueBar.height/6), {255, 0, 0, 255},   {255, 255, 0, 255});
    DrawRectangleGradientV(m_HueBar.x, m_HueBar.y + (m_HueBar.height/6),   m_HueBar.width, std::ceil(m_HueBar.height/6), {255, 255, 0, 255}, {0, 255, 0, 255});
    DrawRectangleGradientV(m_HueBar.x, m_HueBar.y + 2*(m_HueBar.height/6), m_HueBar.width, std::ceil(m_HueBar.height/6), {0, 255, 0, 255},   {0, 255, 255, 255});
    DrawRectangleGradientV(m_HueBar.x, m_HueBar.y + 3*(m_HueBar.height/6), m_HueBar.width, std::ceil(m_HueBar.height/6), {0, 255, 255, 255}, {0, 0, 255, 255});
    DrawRectangleGradientV(m_HueBar.x, m_HueBar.y + 4*(m_HueBar.height/6), m_HueBar.width, std::ceil(m_HueBar.height/6), {0, 0, 255, 255},   {255, 0, 255, 255});
    DrawRectangleGradientV(m_HueBar.x, m_HueBar.y + 5*(m_HueBar.height/6), m_HueBar.width, std::ceil(m_HueBar.height/6), {255, 0, 255, 255}, {255, 0, 0, 255});

    // i actually do not understand this but thanks raysan5 for putting this in raygui
    DrawRectangleGradientEx(m_ColorPicker, WHITE, WHITE, maxHue, maxHue);
    DrawRectangleGradientEx(m_ColorPicker, BLANK, BLACK, BLACK, BLANK);

    Gui.RenderRectangle({m_Picker.x - 5.0f * Gui.GetScale(), m_Picker.y - 5.0f * Gui.GetScale(), 10.0f * Gui.GetScale(), 10.0f * Gui.GetScale()}, 2, 0x000000000, 0xffffffff);

    DrawRectangleRec({440, 270, 40, 40}, ColorFromHSV(m_Color.x, m_Color.y, m_Color.z));

    // hue picker
    Gui.RenderRectangle({m_HuePicker.x, m_HuePicker.y - 2.0f * Gui.GetScale(), m_HueBar.width, 4.0f * Gui.GetScale()}, 2, 0xffffffff, 0x00000000);
}
