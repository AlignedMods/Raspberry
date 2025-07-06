#include "renderer.hpp"
#include "shared/types.hpp"
#include "shared/log.hpp"

#include "raylib.h"
#include "raygui.h"

void s_Renderer::RenderTexture(Texture2D texture, float x, float y) {
	if (texture.width != texture.height) {
		Error("Textures must be square!", -2);
	} else if (texture.width != 16) {
		Error("Textures must be 16x16!", -3);
	} else {
		DrawTextureEx(texture, {x, y}, 0.0f, 4.0f, WHITE);
	}
}

bool s_Renderer::RenderButton(Rectangle rec, std::string text) {
	return GuiButton(rec, text.c_str());
}

void s_Renderer::RenderTextBox(Rectangle bounds, char* txt) {
    GuiTextBox(bounds, txt, GuiGetStyle(DEFAULT, TEXT_SIZE), true);
}

void s_Renderer::RenderTile(std::string tileType, TilePosition) {
}

void s_Renderer::RenderText(const char* txt, int x, int y) {
	GuiLabel(Rectangle{static_cast<float>(x), static_cast<float>(y), 21 * 64, 70}, txt);
}
