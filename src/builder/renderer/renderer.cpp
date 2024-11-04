#include "renderer.hpp"
#include "core/core.hpp"
#include "raylib.h"
#include "raygui.h"
#include "tile/tile.hpp"

#include <iostream>

void Renderer::RenderTexture(Texture2D texture, float x, float y) {
	if (texture.width != texture.height) {
		std::cout << texture.width << " " << texture.height;
		Error("Textures must be square!", -2);
	} else if (texture.width != 16) {
		Error("Textures must be 16x16!", -3);
	} else {
		DrawTextureEx(texture, {x, y}, 0.0f, 4.0f, WHITE);
	}
}

bool Renderer::RenderButton(Rectangle bounds, const char* txt) {
	return GuiButton(bounds, txt);
}

void Renderer::RenderTile(TileType tileType, int x, int y) {
}

void Renderer::RenderText(const char* txt, int x, int y) {
	GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 21 * 64, 40}, txt);
}
