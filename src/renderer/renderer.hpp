#pragma once

#include "raylib.h"

class s_Renderer {
public:
    s_Renderer();

    void RenderTexture(const Texture& texture, float x, float y, Color tint);
    void RenderText(const char* txt, int x, int y);
    void RenderEntity(const Texture& texture, Vector2 position);

	// Prepares the renderer for rendering
	// should be called every frame
	void Begin();

	// Actually does all the rendering
	// also should be called every frame
	void End();
};

extern s_Renderer Renderer;
