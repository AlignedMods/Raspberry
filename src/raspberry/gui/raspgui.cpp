#include "raspGui.hpp"

#include "raylib.h"
#include "shared/log.hpp"

#include <cstddef>

Button::Button(const Vector2& position, const Vector2& size, float fontSize, const std::string& text, bool useScale) {
    Info::CurrentFont = GetFontDefault();

    if (useScale) {
        // Setting correct position
        m_Position.x = position.x * GetScreenWidth();
        m_Position.y = position.y * GetScreenHeight();

        // Setting correct size
        m_Size.x = size.x * GetScreenWidth();
        m_Size.y = size.y * GetScreenHeight();

        // Setting correct font size (according to button size)
        m_FontSize = 40;
    } else {
        m_Position = position;
        m_Size = size;
        m_FontSize = fontSize;
    }

    m_UseScale = useScale;

    m_Rectangle = {m_Position.x, m_Position.y, m_Size.x, m_Size.y};

    m_Text = text;
}

Button::Button(const Vector2& position, const Vector2& size, float fontSize,const std::string& text, bool useScale, std::function<void()> callback) {
    Info::CurrentFont = GetFontDefault();
    m_Text = text;

    if (useScale) {
        // Setting correct position
        m_Position.x = position.x * GetScreenWidth();
        m_Position.y = position.y * GetScreenHeight();

        // Setting correct size
        m_Size.x = size.x * GetScreenWidth();
        m_Size.y = size.y * GetScreenHeight();

        // Setting correct font size (according to button size)
        m_FontSize = 40;
    } else {
        m_Position = position;
        m_Size = size;
        m_FontSize = fontSize;
    }
    m_UseScale = useScale;

    m_Rectangle = {m_Position.x, m_Position.y, m_Size.x, m_Size.y};

    Debug(m_Size.x);

    SetCallback(callback);

    
}

void Button::SetCallback(std::function<void()> callback) {
    m_Callback = callback;

    m_IsCallbackSet = true;
}

void Button::OnUpdate() {
    //Rec2d realRec;

    //if (m_UseScale) {
    //    realRec = Rec2d(m_Position.x * GetScreenWidth(), m_Position.y * GetScreenHeight(), m_Size.x * GetScreenWidth(), m_Size.y * GetScreenHeight());
    //} else {
    //    realRec = m_Rectangle;
    //}

    if (CheckCollisionPointRec(GetMousePosition(), m_Rectangle)) {
        m_Hovered = true;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            m_Clicked = true;
            m_Callback();
        } else { m_Clicked = false; }
    } else { m_Hovered = false; }
}

float Button::GetTextWidth() {
    #if !defined(ICON_TEXT_PADDING)
        #define ICON_TEXT_PADDING   4
    #endif

    Vector2 textSize = { 0 };
    int textIconOffset = 0;

    textSize = MeasureTextEx(Info::CurrentFont, m_Text.c_str(), m_FontSize, Info::Spacing);

    if (textIconOffset > 0) textSize.x += (16 + ICON_TEXT_PADDING);

    return textSize.x;
}

float Button::GetTextWidth(float customFontSize) {
    #if !defined(ICON_TEXT_PADDING)
        #define ICON_TEXT_PADDING   4
    #endif

    Vector2 textSize = { 0 };
    int textIconOffset = 0;

    textSize = MeasureTextEx(Info::CurrentFont, m_Text.c_str(), customFontSize, Info::Spacing);

    if (textIconOffset > 0) textSize.x += (16 + ICON_TEXT_PADDING);

    return textSize.x;
}

float Button::GetTextHeight() {
    #if !defined(ICON_TEXT_PADDING)
        #define ICON_TEXT_PADDING   4
    #endif

    Vector2 textSize = { 0 };
    int textIconOffset = 0;


    textSize = MeasureTextEx(Info::CurrentFont, m_Text.c_str(), m_FontSize, Info::Spacing);

    if (textIconOffset > 0) textSize.x += (16 + ICON_TEXT_PADDING);

    return textSize.y;
}

void Button::OnRender() {
    //Rec2d realRec;

    //if (m_UseScale) {
    //    realRec = Rec2d(m_Position.x * GetScreenWidth(), m_Position.y * GetScreenHeight(), m_Size.x * GetScreenWidth(), m_Size.y * GetScreenHeight());
    //} else {
    //    realRec = m_Rectangle;
    //}

    if (m_Hovered) {
        if (m_Clicked) {
            // When the button is clicked
            DrawRectangleRec(m_Rectangle, GREEN);
        } else {
            // When the button is hovered but not clicked
            DrawRectangleRec(m_Rectangle, RED);
        }
    } else {
        // When the button is neither hovered or clicked
        DrawRectangleRec(m_Rectangle, GRAY);
    }

    //Debug(GetTextHeight());

    DrawTextEx(Info::CurrentFont, 
			   m_Text.c_str(), 
			   Vector2(m_Rectangle.x + m_Rectangle.width / 2 - GetTextWidth() / 2,
			   m_Rectangle.y + m_Rectangle.height / 2 - GetTextHeight() / 2 + m_FontSize / 20.0f), m_FontSize, Info::Spacing, WHITE);
}
