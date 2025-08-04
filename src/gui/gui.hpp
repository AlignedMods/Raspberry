#pragma once

#include "core/types.hpp"

// elements
#include "elements/Text.hpp"
#include "elements/Label.hpp"
#include "elements/Button.hpp"
#include "elements/CheckBox.hpp"
#include "elements/Slider.hpp"
#include "elements/ComboBox.hpp"

#include "raylib.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// funny macros
// very useful though
#define ELEMENT(type)    void AddElement(const type& element)

#define ELEMENT_IMPL(type) void Menu::AddElement(const type& element) {\
    m_Elements.push_back(std::make_shared<type>(element)); \
}

enum class Alignment {
    LEFT, RIGHT,
    CENTER,
    BOTTOM, TOP
};

class Menu {
public:
    void OnUpdate();
    void OnRender();

    void SetName(const std::string& name);
    //void AddElement(const Button& element);
    ELEMENT(Button);
    ELEMENT(Text);
    ELEMENT(Label);
    ELEMENT(CheckBox);
    ELEMENT(Slider);
    ELEMENT(ComboBox);

private:
    std::vector<std::shared_ptr<GuiElement>> m_Elements;
    std::string m_Name;
};

class s_Gui {
public:
    void OnUpdate();
    void OnRender();

    Vector2 GetMousePos();
    Rectangle GetRealSize(const PosInfo& info);

    bool IsHovering();

    void SwitchMenu(const std::string& name);

    Menu* GetCurrentMenu();

    f32 GetScale();

    void RenderRectangle(Rectangle rec, u32 outline, Hex fillColor, Hex outlineColor);

public:
    bool m_Hovering = false;

    std::unordered_map<f32, Font> m_Fonts;

private:
    f32 m_Scale = 1.0f;
    Menu* m_CurrentMenu = nullptr;

    Vector2 m_MousePos;
};

inline s_Gui Gui;
