#include "raspGui.hpp"
#include "renderer/renderer.hpp"

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

	int32_t TextBoxPosition = 0;
};

struct GuiRectangle {
	Rectangle rectangle;
	float roundness;
	int segments;
	uint32_t color;
};

struct GuiText {
	const char* text;
	int size;
	Rectangle rectangle;
};

struct RenderThings {
	std::vector<GuiRectangle> Rectangles;
	std::vector<GuiText> Texts;
};

// "global" info, not accesible outside of this file
static Info g_Info;
static RenderThings g_Render;

// Utility functions
Rectangle GetRealSize(Rectangle bounds) {
	return {bounds.x * GetScreenWidth(), bounds.y * GetScreenHeight(),
			bounds.width * GetScreenWidth(), bounds.height * GetScreenHeight()};
}

void NewCanvas() {
	// clear all the rectangles
	g_Render.Rectangles.clear();
	g_Render.Texts.clear();
}

void Render() {
	for (auto& rec : g_Render.Rectangles) {
		DrawRectangleRounded(rec.rectangle, rec.roundness, rec.segments, GetColor(rec.color));
	}

	for (auto& text : g_Render.Texts) {
		int width = MeasureTextEx(g_Info.Fonts.at(g_Info.Font_Size), text.text, g_Info.Font_Size, 3).x;

		DrawTextEx(g_Info.Fonts.at(text.size), text.text, {text.rectangle.x + text.rectangle.width / 2 - width / 2.0f,
				   text.rectangle.y + text.rectangle.height / 2 - text.size / 2.0f}, text.size, 3, WHITE);
	}
}

// Control
void SetColor(ColorOptions option, uint32_t color) {
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

void SetFontSize(int size) {
	g_Info.Font_Size = size;
}

// Basic drawing functions
void RectangluarRectangle(Rectangle bounds, uint32_t fillColor) {
	OutlinedRectangle(bounds, 0, fillColor, 0x12345678);
}

void OutlinedRectangle(Rectangle bounds, int outline, uint32_t fillColor, uint32_t outlineColor) {
	OutlinedRoundedRectangle(bounds, outline, 0.0f, fillColor, outlineColor);
}

void OutlinedRoundedRectangle(Rectangle bounds, int outline, float roundness, uint32_t fillColor, uint32_t outlineColor) {
	// don't bother calculating an outline if there is none
	Rectangle actual = GetRealSize(bounds);
	if (outline != 0.0f) {
		g_Render.Rectangles.push_back({{actual.x - outline / 2.0f, actual.y - outline / 2.0f,
									   actual.width + outline, actual.height + outline},
									   roundness, 25, outlineColor});
	}

	// Draw actual rectangle
	g_Render.Rectangles.push_back({actual, roundness, 25, fillColor});
}

void Text(Rectangle bounds, const char* text) {
	// load font if it already hasn't been loaded
	if (!g_Info.Fonts.contains(g_Info.Font_Size)) {
		g_Info.Fonts[g_Info.Font_Size] = LoadFontEx("Assets/Fonts/comfy-feeling.otf", g_Info.Font_Size, nullptr, 0);
	}

	Rectangle actual = GetRealSize(bounds);
	int fontSize = g_Info.Font_Size;

	g_Render.Texts.push_back({text, fontSize, actual});
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
			OutlinedRoundedRectangle(bounds, 4, 0.2f, g_Info.Clicked_Fill, g_Info.Clicked_Outline);
		} else {
			OutlinedRoundedRectangle(bounds, 4, 0.2f, g_Info.Hovered_Fill, g_Info.Hovered_Outline);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			clicked = true;
		}
	} else {
		OutlinedRoundedRectangle(bounds, 4, 0.2f, g_Info.Default_Fill, g_Info.Default_Outline);
	}

	Text(bounds, text);

	return clicked;
}

// this was way simpler to implement that i thought
// to be fair it doesn't have all the features
// such as moving left and right but you don't need that
bool TextInput(Rectangle bounds, std::string& str) {
	bool enterPressed = false;
	g_Info.TextBoxPosition = 0;

	if (IsKeyPressed(KEY_ENTER)) {
		enterPressed = true;
	}

	if (IsKeyPressed(KEY_LEFT)) {
		g_Info.TextBoxPosition = std::max(g_Info.TextBoxPosition - 1, 0);
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		g_Info.TextBoxPosition = std::min(g_Info.TextBoxPosition + 1, (int32_t)str.size());
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		if (!str.empty()) {
			str.erase(str.end() - 1);
		}
	}

	// handle pasting
	if (IsKeyPressed(KEY_V) && IsKeyDown(KEY_LEFT_CONTROL)) {
		const char* text = GetClipboardText();

		str.append(text);
	}

	char key = GetCharPressed();

	// check if the character is in range
	if (key > 31 && key < 123) {
		str.append(1, key);
	}

	OutlinedRoundedRectangle(bounds, 5, 0.2f, g_Info.Default_Fill, g_Info.Default_Outline);
	Text(bounds, str.c_str());

	return enterPressed;
}

};
