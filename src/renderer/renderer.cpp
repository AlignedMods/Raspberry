#include "renderer.hpp"
#include "core/log.hpp"

#include "gui/raspGui.hpp"
#include "raylib.h"
#include "raymath.h"

static s_Renderer* instance = nullptr;

s_Renderer::s_Renderer() {
    if (!instance)
        instance = this;
    else {
        Log(LogLevel::Critical, "More than one renderer is created!");
        exit(1);
    }
}

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
	ClearBackground(GetColor(RaspGui::Black));
}

void s_Renderer::End() {
	EndDrawing();
	SwapScreenBuffer();
}
