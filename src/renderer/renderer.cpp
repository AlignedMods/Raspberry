#include "renderer.hpp"
#include "core/log.hpp"

#include "raylib.h"
#include "raymath.h"

void s_Renderer::RenderTexture(const Texture& texture, float x, float y, Color tint) {
    if (texture.width != texture.height) {
        Log(LogLevel::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(LogLevel::Error, "Textures must be a multiple of 8x8!");
    } else {
        DrawTextureEx(texture, {x * 64.0f, y * 64.0f}, 0.0f, 64.0f / texture.width, tint);
    }
}

void s_Renderer::RenderEntity(const Texture& texture, Vector2 position, bool flip) {
	if (texture.width != texture.height) {
        Log(LogLevel::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(LogLevel::Error, "Textures must be a multiple of 8x8!");
    } else {
        DrawTextureEx(texture, Vector2Scale(position, 64.0f), 0.0f, 64.0f / texture.width, WHITE);
    }
}

void s_Renderer::RenderText(const char* text, int x, int y) {
	DrawText(text, x, y, 40, BLACK);
}
