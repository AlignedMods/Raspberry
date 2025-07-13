#pragma once

// RaspGui -- An immediate mode gui made only for raspberry
// NOTE: RaspGui uses scaling instead of absolute values

#include "raylib.h"

#include <cstdint>

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

// Control
void GuiColor(ColorOptions option, uint32_t color);

// Basic Drawing
void RectangluarRectangle(Rectangle bounds, uint32_t fillColor);
void OutlinedRectangle(Rectangle bounds, int outline, uint32_t fillColor, uint32_t outlineColor);
void OutlinedRoundedRectangle(Rectangle bounds, int outline, float roundness, uint32_t fillColor, uint32_t outlineColor);
void Text(Rectangle bounds, const char* text);

void Label(Rectangle bounds, const char* text);
bool Button(Rectangle bounds, const char* text);

} // namespace gui;
