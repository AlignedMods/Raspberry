#include "renderer.hpp"
#include "core/log.hpp"
#include "gui/elements/GuiElement.hpp"

#include "raylib.h"
#include "rlgl.h" // i don't know if anyone actually uses this, i know it's used internally but externally i am not sure

static s_Renderer* instance = nullptr;

s_Renderer::s_Renderer() {
    if (!instance)
        instance = this;
    else {
        Log(LogLevel::Critical, "More than one renderer is created!");
        exit(1);
    }
}

void s_Renderer::RenderTexture(const Texture& texture, f32 x, f32 y, Color tint) {
    if (texture.width != texture.height) {
        Log(LogLevel::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(LogLevel::Error, "Textures must be a multiple of 8x8!");
    } else {
	    DrawTexturePro(texture, {0.0f, 0.0f, (float)texture.width, (float)texture.height}, 
								{x * 32.0f, y * 32.0f, 32.0f, 32.0f}, 
                                {0.0f, 0.0f}, 0.0f, WHITE);
    }
}

void s_Renderer::RenderEntity(const Texture& texture, Vector2 position) {
	if (texture.width != texture.height) {
        Log(LogLevel::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(LogLevel::Error, "Textures must be a multiple of 8x8!");
    } else {
	    DrawTexturePro(texture, {0.0f, 0.0f, (float)texture.width, (float)texture.height}, 
								{position.x * 32.0f, position.y * 32.0f, 32.0f, 32.0f}, 
                                {0.0f, 0.0f}, 0.0f, WHITE);
    }
}

void s_Renderer::RenderText(const char* text, i32 x, i32 y) {
	DrawText(text, x, y, 40, BLACK);
}

void s_Renderer::Begin() {
	BeginDrawing();
	ClearBackground(GetColor(Colors::DGray));
}

void s_Renderer::End() {
	EndDrawing();
	SwapScreenBuffer();
}
