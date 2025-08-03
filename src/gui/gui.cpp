#include "gui.hpp"
#include <format>
#include <memory>
#include "game.hpp"
#include "core/log.hpp"
#include "registry.hpp"

#include "raylib.h"

void Execute(const std::string &str);

Button::Button(Rectangle bounds, const std::string& text, const std::string& onClick) : 
    m_Text(text), m_OnClick(onClick) {
    m_Bounds = bounds;
}

void Button::OnUpdate() {
    Log(LogLevel::Info, "Updating!");

    Vector2 mouse = Gui.GetMousePos();
    Rectangle actual = Gui.GetRealSize(m_Bounds);

    if (CheckCollisionPointRec(mouse, actual)) {
        m_State = 1; // Hovered
        Gui.m_Hovering = true;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            m_State = 2; // Clicked
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Execute(m_OnClick);
        }
    } else {
        m_State = 0; // Default
    }
}

void Button::OnRender() {
    Log(LogLevel::Info, "Rendering!");
    Rectangle actual = Gui.GetRealSize(m_Bounds);

    switch (m_State) {
        case 0:
            DrawRectangleRec(actual, GetColor(m_Style.DefaultFill));
            break;
        case 1:
            DrawRectangleRec(actual, GetColor(m_Style.HoveredFill));
            break;
        case 2:
            DrawRectangleRec(actual, GetColor(m_Style.ClickedFill));
            break;
    }
}

void Menu::OnUpdate() {
    for (auto& element : m_Elements) {
        element->OnUpdate();
    }
}

void Menu::OnRender() {
    Log(LogLevel::Info, std::format("{}", m_Elements.size()));
    for (auto& element : m_Elements) {
        Log(LogLevel::Info, std::format("Hllo world"));
        element->OnRender();
    }
}

void Menu::SetName(const std::string& name) {
    m_Name = name;
}

ELEMENT_IMPL(Button)

// void Menu::AddElement(const Button& element) {
//     m_Elements.push_back(std::make_shared<Button>(element));
// }

void s_Gui::OnUpdate() {
    m_Hovering = false;
    m_MousePos = GetMousePosition();

    if (m_CurrentMenu) {
        m_CurrentMenu->OnUpdate();
    }
}

void s_Gui::OnRender() {
    if (m_CurrentMenu) {
        m_CurrentMenu->OnRender();
    }
}

Vector2 s_Gui::GetMousePos() {
    return m_MousePos;
}

Rectangle s_Gui::GetRealSize(const Rectangle& rec) {
    return {rec.x * m_Scale, rec.y * m_Scale, rec.width * m_Scale, rec.height * m_Scale};
}

bool s_Gui::IsHovering() {
    return m_Hovering;
}


void s_Gui::SwitchMenu(const std::string& menu) {
    if (!Registry.DoesMenuExist(menu)) {
        m_CurrentMenu = nullptr;
        return;
    }

    // ensure that we don't set the previous menu before switching to it (causes previous to be the same as current)
    if (menu == "$PREVIOUS") {
        m_CurrentMenu = &Registry.GetMenu(menu);
        Registry.AddMenu("$PREVIOUS", *m_CurrentMenu);
    } else {
        if (m_CurrentMenu) {
            Registry.AddMenu("$PREVIOUS", *m_CurrentMenu);
        }

        m_CurrentMenu = &Registry.GetMenu(menu);
    }
}

Menu* s_Gui::GetCurrentMenu() {
    return m_CurrentMenu;
}

void Execute(const std::string &str) {
    std::vector<std::string> tokens;
    std::string temp;

    u32 index = 0;

    while (index < str.size()) {
        if (str.at(index) != ' ') {
            temp += str.at(index);
        } else {
            tokens.push_back(temp);
            temp.clear();
        }

        index++;
    }

    // if there is something left over in the buffer we want to add it
    if (!temp.empty()) {
        tokens.push_back(temp);
        temp.clear();
    }

    // actual execution
    if (tokens.size() == 0) {
        Log(LogLevel::Error, "Expected command!");
        return;
    }

    if (tokens.at(0) == "switch") {
        if (tokens.size() < 2) {
            Log(LogLevel::Error, "Syntax: switch <menu>");
            return;
        } else {
            Gui.SwitchMenu(tokens.at(1));
        }
    }

    if (tokens.at(0) == "exec-cpp") {
        if (tokens.size() < 2) {
            Log(LogLevel::Error, "Syntax: exec-cpp <function>");
            return;
        } else {
            Registry.CallFunction(tokens.at(1));
        }
    }
}
