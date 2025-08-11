#pragma once

#include "core/types.hpp"

#include "raylib.h"

class s_Renderer {
public:
    s_Renderer();

    void RenderTexture(const Texture& texture, f32 x, f32 y, Color tint);
    void RenderText(const char* txt, f2 pos, u32 fontSize);
    void RenderEntity(const Texture& texture, Vector2 position);

	// Prepares the renderer for rendering
	// should be called every frame
	void Begin();

	// Actually does all the rendering
	// also should be called every frame
	void End();

private:
    Camera2D m_Internal = {
        .offset = {0, 0},
        .target = {0, 0},
        .rotation = 0,
        .zoom = 1
    };
};

extern s_Renderer Renderer;
