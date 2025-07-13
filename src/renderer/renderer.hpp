#pragma once

#include "raylib.h"

class s_Renderer {
public:
    void RenderTexture(const Texture& texture, float x, float y, Color tint);
    void RenderText(const char* txt, int x, int y);
    void RenderEntity(const Texture& texture, Vector2 position, bool flip);
};

inline s_Renderer Renderer;
