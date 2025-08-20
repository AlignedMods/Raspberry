#include "renderer.hpp"
#include "log.hpp"

#include "raylib.h"
#include "raymath.h"

static s_Renderer* instance = nullptr;

s_Renderer::s_Renderer() {
    if (!instance)
        instance = this;
    else {
        Log(Log::Critical, "More than one renderer is created!");
        exit(1);
    }
}

void s_Renderer::RenderTexture(const Texture& texture, f32 x, f32 y, Color tint) {
    if (texture.width != texture.height) {
        Log(Log::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(Log::Error, "Textures must be a multiple of 8x8!");
    } else {
	    DrawTexturePro(texture, {0.0f, 0.0f, (float)texture.width, (float)texture.height}, 
								{x * 32.0f, y * 32.0f, 32.0f, 32.0f}, 
                                {0.0f, 0.0f}, 0.0f, WHITE);
    }
}

void s_Renderer::RenderEntity(const Texture& texture, Vector2 position, bool flip) {
	if (texture.width != texture.height) {
        Log(Log::Error, "Textures must be square!");
    } else if (texture.width % 8 != 0) {
        Log(Log::Error, "Textures must be a multiple of 8x8!");
    } else {
        Rectangle src = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        Rectangle dest = {position.x * 32.0f, position.y * 32.0f, 32.0f, 32.0f};

        if (flip) {
            // flip the texture horizontally
            src.width *= -1;
        }

	    DrawTexturePro(texture, src, 
                                dest,
                                {0.0f, 0.0f}, 0.0f, WHITE);
    }
}

void s_Renderer::RenderText(const char* text, f2 pos, u32 fontSize) {
    f32 spacing = fontSize/10.0f;
    f2 dim = MeasureTextEx(GetFontDefault(), text, fontSize, spacing);

    dim = Vector2Scale(dim, 0.5f);

    dim = Vector2Subtract(pos, dim);

    DrawTextEx(GetFontDefault(), text, {dim.x, dim.y}, fontSize, spacing, WHITE);
}

void s_Renderer::Begin() {
	BeginDrawing();
	ClearBackground(GetColor(0x323232ff));

    // this is a pretty hacky solution but i like it a lot ( not much work :) )
    m_Internal.target = {0.0f + (GetScreenWidth() / 2.0f), 0.0f + (GetScreenHeight() / 2.0f)};
    m_Internal.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    m_Internal.zoom = 1.0f / (360.0f / GetScreenHeight());

    BeginMode2D(m_Internal);
}

void s_Renderer::End() {
    EndMode2D();

	EndDrawing();
	SwapScreenBuffer();
}
