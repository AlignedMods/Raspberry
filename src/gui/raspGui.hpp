#pragma once

// RaspGui -- An immediate mode gui made only for raspberry
// NOTE: RaspGui uses scaling instead of absolute values

#include "raylib.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace RaspGui {

using Hex = uint32_t;

// The Raspberry color palette™

// The RGB trilogy
inline Hex Red    = 0xf25757ff;
inline Hex Green  = 0x69f257ff;
inline Hex Blue   = 0x4287f5ff;

// The colorless
inline Hex Black  = 0x141414ff;
inline Hex DGray  = 0x3d3d3dff;
inline Hex Gray   = 0x696969ff;
inline Hex LGray  = 0x9c9c9cff;
inline Hex White  = 0xe0e0e0ff;

// Based off red
inline Hex Yellow = 0xfcfc4cff;
inline Hex Orange = 0xfca44cff;

// Based off blue
inline Hex Cyan   = 0x5edfffff;
inline Hex Purple = 0x954cfcff;

// Based off green
inline Hex Teal   = 0x55fac0ff;
inline Hex Lime   = 0x25cc51ff;

struct Pallete {
    Hex DefaultFill    = DGray;
    Hex DefaultOutline = Red;

    Hex HoveredFill    = Gray;
    Hex HoveredOutline = Red;

    Hex ClickedFill    = LGray;
    Hex ClickedOutline = Red;

    int Outline        = 4;
};

struct Input {
    std::string Text;
    int32_t Position = 0;
};

enum class ButtonInput {
    Click, Hold
};

enum class Behaviour {
    Default, Custom
};

// Utility functions
Rectangle GetRealSize(Rectangle bounds);

bool HoveringOverGui();

// Rendering only functions (you do not *need* to use these, just nothing will be displayed if you don't)
void NewCanvas();
void Render();

// Basic Drawing
void RectangluarRectangle(Rectangle bounds, uint32_t fillColor);
void OutlinedRectangle(Rectangle bounds, int outline, uint32_t fillColor, uint32_t outlineColor);
void OutlinedRoundedRectangle(Rectangle bounds, int outline, float roundness, uint32_t fillColor, uint32_t outlineColor);

void Window(Rectangle& bounds, const char* name);
void WindowEx(Rectangle& bounds, const char* name, const Pallete& pallete);

// Panels
void Panel(Rectangle bounds);
void PanelEx(Rectangle bounds, const Pallete& pallete);

void End();

// Text
void Text(Rectangle bounds, const char* text);

void Label(Rectangle bounds, const char* text);
void LabelEx(Rectangle bounds, const char* text, const Pallete& pallete);

// Buttons (not specifying a pallete will use the default one)
bool Button(Rectangle bounds, const char* text);
bool ButtonEx(Rectangle bounds, const char* text, const Pallete& pallete);
bool ButtonPro(Rectangle bounds, const char* text, const Pallete& pallete, ButtonInput input);

// returns true when enter is pressed
// You must provide an existing input object to be used
bool TextInput(Rectangle bounds, Input& input);
bool TextInputEx(Rectangle bounds, Input& input, const Pallete& pallete);

void ComboBox(Rectangle bounds, const std::string& options, uint32_t& selection, Behaviour behavoir = Behaviour::Default);
void ComboBoxEx(Rectangle bounds, const std::string& options, uint32_t& selection, Behaviour behavoir,const Pallete& pallete);

} // namespace gui;
