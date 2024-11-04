#pragma once

#include "raylib.h"

class Renderer {
public:
	void RenderTexture(Texture2D texture, float x, float y);
	void RenderText(const char* txt, int x, int y);
};
