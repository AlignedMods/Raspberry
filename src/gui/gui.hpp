#pragma once

#include "core/log.hpp"
#include "core/types.hpp"
#include "raylib.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// funny macros
// very useful though
#define ELEMENT(type)      void AddElement(const type& element)

#define ELEMENT_IMPL(type) void Menu::AddElement(const type& element) {\
    m_Elements.push_back(std::make_shared<type>(element)); \
}

using Hex = u32;

// The Raspberry color palette™

namespace Colors {

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

}

struct PosInfo {
    Rectangle Bounds;
    std::string AnchorX;
    std::string AnchorY;
};

struct Style {
    Hex DefaultFill = Colors::DGray;
    Hex DefaultOutline = Colors::Teal;
    Hex HoveredFill = Colors::Gray;
    Hex HoveredOutline = Colors::Teal;
    Hex ClickedFill = Colors::LGray;
    Hex ClickedOutline = Colors::Teal;

    f32 Roundness = 0.0f;
    u32 Outline = 2;
};

class GuiElement {
public:
    virtual void OnUpdate() {}
    virtual void OnRender() {}

protected:
    PosInfo m_Bounds;
    Style m_Style;
};

class Text : public GuiElement {
public:
    Text(const PosInfo& info, const std::string& text);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    std::string m_Text;
    f32 m_FontSize = 0.0f;
    f32 m_BaseFontSize = 100.0f;

    f32 m_TextWidth = 0.0f;
    f32 m_TextHeight = 0.0f;

    bool m_Calculated = false;

    Vector2 m_Padding = {1.2f, 1.2f};
};

class Button : public GuiElement {
public:
    Button(const PosInfo& info, const std::string& text, const std::string& onClick);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    std::string m_OnClick;
    Text m_Text;

    u32 m_State = 0;
};

class Menu {
public:
    void OnUpdate();
    void OnRender();

    void SetName(const std::string& name);
    //void AddElement(const Button& element);
    ELEMENT(Button);
    ELEMENT(Text);

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

public:
    bool m_Hovering = false;

    std::unordered_map<f32, Font> m_Fonts;

private:
    f32 m_Scale = 1.25f;
    Menu* m_CurrentMenu = nullptr;

    Vector2 m_MousePos;
};

inline s_Gui Gui;
