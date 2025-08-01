#pragma once

#include "raylib.h"

#include <string>
#include <vector>

struct Button {
    Rectangle Bounds;
    std::string Text;
    std::string OnClick;
};

struct Label {
    Rectangle Bounds;
    std::string Text;
};

struct Menu {
    std::vector<Button> Buttons;
    std::vector<Label> Labels;
};

void Execute(const std::string& str);
