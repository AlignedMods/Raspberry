#include "raspGui.hpp"
#include "raylib.h"

Button::Button(const Vec2d& position, const Vec2d& size, const std::string& text, bool useScale) {
    m_Position = position;
    m_Size = size;
    m_UseScale = useScale;

    m_Rectangle = Rec2d(m_Position, m_Size);

    m_Text = text;
}

Button::Button(const Vec2d& position, const Vec2d& size, const std::string& text, bool useScale, std::function<void()> callback) {
    m_Position = position;
    m_Size = size;
    m_UseScale = useScale;

    m_Rectangle = Rec2d(m_Position, m_Size);
    m_Text = text;

    SetCallback(callback);
}

void Button::SetCallback(std::function<void()> callback) {
    m_Callback = callback;

    m_IsCallbackSet = true;
}

void Button::OnUpdate() {
    Rec2d realRec;

    if (m_UseScale) {
        realRec = Rec2d(m_Position.x * GetScreenWidth(), m_Position.y * GetScreenHeight(), m_Size.x * GetScreenWidth(), m_Size.y * GetScreenHeight());
    } else {
        realRec = m_Rectangle;
    }

    if (CheckCollisionPointRec(GetMousePosition(), realRec.ToRaylibrectangle())) {
        m_Hovered = true;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            m_Clicked = true;
            m_Callback();
        } else { m_Clicked = false; }
    } else { m_Hovered = false; }
}

void Button::OnRender() {
    Rec2d realRec;

    if (m_UseScale) {
        realRec = Rec2d(m_Position.x * GetScreenWidth(), m_Position.y * GetScreenHeight(), m_Size.x * GetScreenWidth(), m_Size.y * GetScreenHeight());
    } else {
        realRec = m_Rectangle;
    }

    if (m_Hovered) {
        if (m_Clicked) {
            // When the button is clicked
            DrawRectangleRec(realRec.ToRaylibrectangle(), GREEN);
        } else {
            // When the button is hovered but not clicked
            DrawRectangleRec(realRec.ToRaylibrectangle(), RED);
        }
    } else {
        // When the button is neither hovered or clicked
        DrawRectangleRec(realRec.ToRaylibrectangle(), GRAY);
    }
}