#pragma once

#include "core/types.hpp"

#include <functional>

// stolen pretty much directly from DSDA-doom menu source code
// I just made it more applicable to C++, instead of C

struct MenuItem {
    u16 type; // type of item
    char text[255]; // text to be displayed
    std::function<void()> callback; // function to execute on select
    f32 fontSize = 40;
    u32 y = 0;

    f32 timeSinceSelect = 0.0f;
};

struct Menu {
    u16 numItems;
    MenuItem* items;
    Menu* prevMenu;
    u16 selection;
    
    std::function<void()> update = nullptr;
    std::function<void()> render = nullptr;
};

// also not everything has to be a class, this makes more sense to me
extern void InitMenu();

void UpdateCurrentMenu();
void RenderCurrentMenu();
void SwitchMenu(Menu* menu);

// gurt
