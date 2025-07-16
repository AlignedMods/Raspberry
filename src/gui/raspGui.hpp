#pragma once

// RaspGui -- An immediate mode gui made only for raspberry
// NOTE: RaspGui uses scaling instead of absolute values

#include "raylib.h"

#include <cstdint>
#include <string>

namespace RaspGui {

enum class ColorOptions {
	DefaultFill,
	DefaultOutline,
	HoveredFill,
	HoveredOutline,
	ClickedFill,
	ClickedOutline
};

// Utility functions
Rectangle GetRealSize(Rectangle bounds);

// Rendering only functions (you do not *need* to use these, just nothing will be displayed if you don't)
void NewCanvas();
void Render();

// Control
void SetColor(ColorOptions option, uint32_t color);
void SetFontSize(int size);

// Basic Drawing
void RectangluarRectangle(Rectangle bounds, uint32_t fillColor);
void OutlinedRectangle(Rectangle bounds, int outline, uint32_t fillColor, uint32_t outlineColor);
void OutlinedRoundedRectangle(Rectangle bounds, int outline, float roundness, uint32_t fillColor, uint32_t outlineColor);
void Text(Rectangle bounds, const char* text);

void Label(Rectangle bounds, const char* text);
bool Button(Rectangle bounds, const char* text);
bool Button(Rectangle bounds, const char* text, uint32_t fillColor, uint32_t outlineColor);

// returns true when enter is pressed
// You must provide an existing string to be used
bool TextInput(Rectangle bounds, std::string& str);

} // namespace gui;
