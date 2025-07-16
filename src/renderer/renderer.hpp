#pragma once

#include "raylib.h"

#include <vector>
#include <cstdint>


class s_Renderer {
public:
    void RenderTexture(const Texture& texture, float x, float y, Color tint);
    void RenderText(const char* txt, int x, int y);
    void RenderEntity(const Texture& texture, Vector2 position, bool flip);

	// Prepares the renderer for rendering
	// should be called every frame
	void Begin();

	// Actually does all the rendering
	// also should be called every frame
	void End();
};

inline s_Renderer Renderer;
