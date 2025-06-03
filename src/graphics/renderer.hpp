#pragma once

#include "raylib.h"
#include "utils/types.hpp"

class Renderer {
    public:
    void RenderTexture(const Texture& texture, float x, float y);
    void RenderText(const char* txt, int x, int y);
    void RenderEntity(const Texture& texture, Vec2d position, bool flip);
};
