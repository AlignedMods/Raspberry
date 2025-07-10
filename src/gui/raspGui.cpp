#include "raspGui.hpp"

#include "raylib.h"

#include <unordered_map>

namespace RaspGui {

struct Info {
	// Colors
	uint32_t Default_Fill = 0xffb0d2ff;
	uint32_t Default_Outline = 0x96687cff;

	uint32_t Hovered_Fill = 0xbf5885ff;
	uint32_t Hovered_Outline = 0x96687cff;

	uint32_t Clicked_Fill = 0x8a3e5fff;
	uint32_t Clicked_Outline = 0x692f48ff;

	int Font_Size = 70;
	std::unordered_map<int, Font> Fonts;
};

// "global" info, not accesible outside of this file
static Info g_Info;

// Utility functions
Rectangle GetRealSize(Rectangle bounds) {
	return {bounds.x * GetScreenWidth(), bounds.y * GetScreenHeight(),
			bounds.width * GetScreenWidth(), bounds.height * GetScreenHeight()};
}

// Control
void GuiColor(ColorOptions option, uint32_t color) {
	switch (option) {
		case ColorOptions::DefaultFill:
			g_Info.Default_Fill = color;
			break;
		case ColorOptions::DefaultOutline:
			g_Info.Default_Outline = color;
			break;
		case ColorOptions::HoveredFill:
			g_Info.Hovered_Fill = color;
			break;
		case ColorOptions::HoveredOutline:
			g_Info.Hovered_Outline = color;
			break;
		case ColorOptions::ClickedFill:
			g_Info.Clicked_Fill = color;
			break;
		case ColorOptions::ClickedOutline:
			g_Info.Clicked_Outline = color;
			break;
	}
}

// Basic drawing functions
void OutlinedRectangle(Rectangle bounds, int outline, uint32_t fillColor, uint32_t outlineColor) {
	OutlinedRoundedRectangle(bounds, outline, 0.0f, fillColor, outlineColor);
}

void OutlinedRoundedRectangle(Rectangle bounds, int outline, float roundness, uint32_t fillColor, uint32_t outlineColor) {
	// don't bother calculating an outline if there is none
	Rectangle actual = GetRealSize(bounds);
	if (outline != 0.0f) {
		// Draw outline
		::DrawRectangleRounded({actual.x - outline / 2.0f, actual.y - outline / 2.0f,
								actual.width + outline, actual.height + outline}, 
								roundness, 50, GetColor(outlineColor));
	}

	// Draw actual rectangle
	::DrawRectangleRounded(actual, roundness, 50, GetColor(fillColor));
}

void Text(Rectangle bounds, const char* text) {
	// load font if it already hasn't been loaded
	if (!g_Info.Fonts.contains(g_Info.Font_Size)) {
		g_Info.Fonts[g_Info.Font_Size] = LoadFontEx("Assets/Fonts/comfy-feeling.otf", g_Info.Font_Size, nullptr, 0);
	}

	Rectangle actual = GetRealSize(bounds);

	int width = MeasureTextEx(g_Info.Fonts.at(g_Info.Font_Size), text, g_Info.Font_Size, 3).x;

	::DrawTextEx(g_Info.Fonts.at(g_Info.Font_Size), text, {actual.x + actual.width / 2 - width / 2.0f,
			   actual.y + actual.height / 2 - g_Info.Font_Size / 2.0f}, g_Info.Font_Size, 3, WHITE);
}

void Label(Rectangle bounds, const char* text) {
	OutlinedRectangle(bounds, 5, g_Info.Default_Fill, g_Info.Default_Outline);

	Text(bounds, text);
}

bool Button(Rectangle bounds, const char* text) {
	Vector2 mousePos = GetMousePosition();
	Rectangle actual = GetRealSize(bounds);

	bool clicked = false;

	// Hovering over the button
	if (CheckCollisionPointRec(mousePos, actual)) {
		// button is clicked
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			OutlinedRectangle(bounds, 4, g_Info.Clicked_Fill, g_Info.Clicked_Outline);
		} else {
			OutlinedRectangle(bounds, 4, g_Info.Hovered_Fill, g_Info.Hovered_Outline);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			clicked = true;
		}
	} else {
		OutlinedRectangle(bounds, 4, g_Info.Default_Fill, g_Info.Default_Outline);
	}

	Text(bounds, text);

	return clicked;
}

};
