#pragma once

#include "raylib.h"
#include "tile/tile.hpp"

class Renderer {
public:
	void RenderTexture(Texture2D texture, float x, float y);
	bool RenderButton(Rectangle bounds, const char* txt);
	void RenderText(const char* txt, int x, int y);

	void RenderTile(TileType tileType, int x, int y);
};
