#include "renderer.hpp"
#include "core/log.hpp"

#include "gui/raspGui.hpp"
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

void s_Renderer::RenderEntity(const Texture& texture, Vector2 position) {
	if (texture.width != texture.height) {
        Log(LogLevel::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(LogLevel::Error, "Textures must be a multiple of 8x8!");
    } else {
	    DrawTexturePro(texture, {0.0f, 0.0f, (float)texture.width, (float)texture.height}, 
								{position.x * 64.0f, position.y * 64.0f, 64.0f, 64.0f}, 
								{0.0f, 0.0f}, 0.0f, WHITE);
    }
}

void s_Renderer::RenderText(const char* text, int x, int y) {
	DrawText(text, x, y, 40, BLACK);
}

void s_Renderer::Begin() {
	BeginDrawing();
	ClearBackground(WHITE);
}

void s_Renderer::End() {
	RaspGui::NewCanvas();

	EndDrawing();
	SwapScreenBuffer();
}
