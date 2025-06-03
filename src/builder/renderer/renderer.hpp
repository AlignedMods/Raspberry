#pragma once

#include <string>
#include "raylib.h"
#include "utils/types.hpp"

class Renderer {
public:
	void RenderTexture(Texture2D texture, float x, float y);
	bool RenderButton(Vec2d position, Vec2d size, std::string text);
	void RenderText(const char* txt, int x, int y);
    void RenderTextBox(Rectangle bounds, char* txt);

	void RenderTile(std::string tileType, int x, int y);
};
