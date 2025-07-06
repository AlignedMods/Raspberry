#pragma once

#include "shared/types.hpp"

#include "raylib.h"

#include <string>

class s_Renderer {
public:
	void RenderTexture(Texture2D texture, float x, float y);
	bool RenderButton(Rectangle rec, std::string text);
	void RenderText(const char* txt, int x, int y);
    void RenderTextBox(Rectangle bounds, char* txt);

	void RenderTile(std::string tileType, TilePosition pos);
};

inline s_Renderer Renderer;
