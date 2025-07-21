#include "raspGui.hpp"
#include "core/log.hpp"
#include "game.hpp"
#include "raymath.h"
#include "renderer/renderer.hpp"

#include "raylib.h"

#include <cstddef>
#include <format>
#include <string>
#include <unordered_map>

namespace RaspGui {

struct Info {
	int Font_Size = 40;
	std::unordered_map<int, Font> Fonts;

    std::vector<int> TextInputPositions;
    int TextBoxPosition;

	bool Hovering = false;

    std::vector<Rectangle> Windows;
    size_t WindowCount = 0;
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

static Pallete g_DefaultPallete;

// Utility functions
Rectangle GetRealSize(Rectangle bounds) {
	return {bounds.x * GetScreenWidth(), bounds.y * GetScreenHeight(),
			bounds.width * GetScreenWidth(), bounds.height * GetScreenHeight()};
}

bool HoveringOverGui() {
	return g_Info.Hovering;
}

void NewCanvas() {
	// clear all the rectangles
	g_Render.Rectangles.clear();
	g_Render.Texts.clear();

    g_Info.WindowCount = 0;
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
    Rectangle outlineRec = {actual.x - outline / 2.0f, actual.y - outline / 2.0f,
                         actual.width + outline, actual.height + outline};

	if (outline != 0.0f) {
		g_Render.Rectangles.push_back({outlineRec, roundness, 25, outlineColor});
	}

    if (CheckCollisionPointRec(GetMousePosition(), outlineRec)) {
        g_Info.Hovering = true;
    } else {
        g_Info.Hovering = false;
    }

	// Draw actual rectangle
	g_Render.Rectangles.push_back({actual, roundness, 25, fillColor});
}

void Window(Rectangle& bounds, const char* name) {
    Window(bounds, name, g_DefaultPallete);
}

void Window(Rectangle& bounds, const char* name, const Pallete& pallete) {
    if (Button({bounds.x, bounds.y - 0.05f, bounds.width, 0.05f}, name, pallete, ButtonInput::Hold)) {
        Vector2 delta = Vector2Scale(GetMouseDelta(), 1.0f);

        bounds.x += delta.x;
        bounds.y += delta.y;
    }

    OutlinedRectangle(bounds, pallete.Outline, pallete.DefaultFill, pallete.DefaultOutline);
}

void Panel(Rectangle bounds) {
    Panel(bounds, g_DefaultPallete);
}

void Panel(Rectangle bounds, const Pallete& pallete) {
    OutlinedRectangle(bounds, pallete.Outline, pallete.DefaultFill, pallete.DefaultOutline);
}

void Text(Rectangle bounds, const char* text) {
	// load font if it already hasn't been loaded
	if (!g_Info.Fonts.contains(g_Info.Font_Size)) {
		g_Info.Fonts[g_Info.Font_Size] = LoadFontEx("Assets/Fonts/alagard.ttf", g_Info.Font_Size, nullptr, 0);
	}

	Rectangle actual = GetRealSize(bounds);
	int fontSize = g_Info.Font_Size;

	g_Render.Texts.push_back({text, fontSize, actual});
}

void Label(Rectangle bounds, const char* text) {
    Label(bounds, text, g_DefaultPallete);
}

void Label(Rectangle bounds, const char* text, const Pallete& pallete) {
	OutlinedRectangle(bounds, pallete.Outline, pallete.DefaultFill, pallete.DefaultOutline);

	Text(bounds, text);
}

bool Button(Rectangle bounds, const char* text) {
    return Button(bounds, text, g_DefaultPallete);
}

bool Button(Rectangle bounds, const char* text, const Pallete& pallete) {
    return Button(bounds, text, pallete, ButtonInput::Click);
}

bool Button(Rectangle bounds, const char* text, const Pallete& pallete, ButtonInput input) {
	Vector2 mousePos = GetMousePosition();
	Rectangle actual = GetRealSize(bounds);

	bool clicked = false;

	// Hovering over the button
	if (CheckCollisionPointRec(mousePos, actual)) {
		// button is clicked
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (input == ButtonInput::Hold) clicked = true;
			OutlinedRectangle(bounds, pallete.Outline, pallete.ClickedFill, pallete.ClickedOutline);
		} else {
			OutlinedRectangle(bounds, pallete.Outline, pallete.HoveredFill, pallete.HoveredOutline);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (input == ButtonInput::Click) clicked = true;
		}
	} else {
		OutlinedRectangle(bounds, pallete.Outline, pallete.DefaultFill, pallete.DefaultOutline);
		g_Info.Hovering = false;
	}

	Text(bounds, text);

	return clicked;
}

bool TextInput(Rectangle bounds, Input& input) {
    return TextInput(bounds, input, g_DefaultPallete);
}

// this was way simpler to implement that i thought
// to be fair it doesn't have all the features
// such as moving left and right but you don't need that
bool TextInput(Rectangle bounds, Input& input, const Pallete& pallete) {
	bool enterPressed = false;

	if (IsKeyPressed(KEY_ENTER)) {
		enterPressed = true;
	}

	if (IsKeyPressed(KEY_LEFT)) {
		input.Position = std::max(input.Position - 1, 0);
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		input.Position = std::min(input.Position + 1, (int32_t)input.Text.length() - 1);
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
        if (input.Position > 0) {
            input.Text.erase(input.Position - 1, 1);
            input.Position--;
        }
	}

	// handle pasting
	if (IsKeyPressed(KEY_V) && IsKeyDown(KEY_LEFT_CONTROL)) {
		const char* text = GetClipboardText();

		input.Text.append(text);
	}

	char key = GetCharPressed();

	// check if the character is in range
	if (key > 31 && key < 123) {
        input.Text.insert(input.Position, 1, key);
        input.Position++;
	}

    Log(LogLevel::Info, std::format("{}", input.Position));

	OutlinedRectangle(bounds, pallete.Outline, pallete.DefaultFill, pallete.DefaultOutline);
	Text(bounds, input.Text.c_str());

	return enterPressed;
}

};
