#include "renderer.hpp"

#include "core/core.hpp"
#include "raylib.h"
#include "utils/types.hpp"

void Renderer::RenderTexture(const Texture& texture, float x, float y) {
    if (texture.width != texture.height) {
        Error("Textures must be square!", -2);
    } else if (texture.width % 8 != 0) {
        Error("Textures must be a multiple of 8x8!", -3);
    } else {
        DrawTextureEx(texture, {x, y}, 0.0f, 64.0f / texture.width, WHITE);
    }
}

void Renderer::RenderEntity(const Texture& texture, Vec2d position, bool flip) {
    if (texture.width != texture.height) {
        Error("Textures must be square!", -2);
    } else if (texture.width % 8 != 0) {
        Error("Textures must be a multiple of 8x8!", -3);
    } else {
        DrawTextureEx(texture, position.ToRaylibVector(), 0.0f, 64.0f / texture.width, WHITE);
    }
}