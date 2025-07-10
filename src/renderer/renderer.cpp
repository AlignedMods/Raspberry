#include "renderer.hpp"
#include "core/log.hpp"

#include "raylib.h"

void s_Renderer::RenderTexture(const Texture& texture, float x, float y) {
    if (texture.width != texture.height) {
        Log(LogLevel::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(LogLevel::Error, "Textures must be a multiple of 8x8!");
    } else {
        DrawTextureEx(texture, {x, y}, 0.0f, 64.0f / texture.width, WHITE);
    }
}

void s_Renderer::RenderEntity(const Texture& texture, Vector2 position, bool flip) {
	if (texture.width != texture.height) {
        Log(LogLevel::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(LogLevel::Error, "Textures must be a multiple of 8x8!");
    } else {
        DrawTextureEx(texture, position, 0.0f, 64.0f / texture.width, WHITE);
    }
}
