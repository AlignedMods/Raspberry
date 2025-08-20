#pragma once

#include "raylib.h"
#include "raymath.h"

#include <string>

class EditorTile {
public:
    EditorTile() = default;
    inline EditorTile(const Vector2& pos, const std::string& type) {
        m_Pos = pos;
        m_Type = type;

        // m_Texture = Registry.GetTexture(type);
    }

    inline Vector2 GetPosition() {
        return m_Pos;
    }

    inline void OnRender() {
        // DrawTextureV(m_Texture, m_Pos, WHITE);
        DrawRectangleV(Vector2Scale(m_Pos, 32.0f), {32.0f, 32.0f}, BLACK);
    }

private:
    Vector2 m_Pos;
    std::string m_Type;

    Texture m_Texture;
};
