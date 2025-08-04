#include "gui.hpp"
#include <format>
#include "core/log.hpp"
#include "registry.hpp"

#include "raylib.h"

void Menu::OnUpdate() {
    for (auto& element : m_Elements) {
        element->OnUpdate();
    }
}

void Menu::OnRender() {
    for (auto& element : m_Elements) {
        element->OnRender();
    }
}

void Menu::SetName(const std::string& name) {
    m_Name = name;
}

// i don't wanna repeat the same bullshit like 20 times
ELEMENT_IMPL(Button)
ELEMENT_IMPL(Text)
ELEMENT_IMPL(Label)
ELEMENT_IMPL(CheckBox)
ELEMENT_IMPL(Slider)
ELEMENT_IMPL(ComboBox)

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

Rectangle s_Gui::GetRealSize(const PosInfo& info) {
    Rectangle rec;

    rec.width = info.Bounds.width * m_Scale;
    rec.height = info.Bounds.height * m_Scale;

    if (info.AnchorX == "left") {
        rec.x = info.Bounds.x * m_Scale;
    }

    if (info.AnchorX == "middle") {
        rec.x = GetScreenWidth() / 2.0f - rec.width / 2.0f + info.Bounds.x * m_Scale;
    }

    if (info.AnchorX == "right") {
        rec.x = GetScreenWidth() - rec.width + info.Bounds.x * m_Scale;
    }

    if (info.AnchorY == "top") {
        rec.y = info.Bounds.y * m_Scale;
    }

    if (info.AnchorY == "middle") {
        rec.y = GetScreenHeight() / 2.0f - rec.height / 2.0f + info.Bounds.y * m_Scale;
    }

    if (info.AnchorY == "bottom") {
        rec.y = GetScreenHeight() - rec.height - info.Bounds.y * m_Scale;
    }

    return rec;
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

        if (menu == "Quit-Menu") {
            Log(LogLevel::Info, std::format("{}", (u64)&Registry.GetMenu(menu)));
        }

        m_CurrentMenu = &Registry.GetMenu(menu);
    }
}

Menu* s_Gui::GetCurrentMenu() {
    return m_CurrentMenu;
}

f32 s_Gui::GetScale() {
    return m_Scale;
}

void s_Gui::RenderRectangle(Rectangle rec, u32 outline, Hex fillColor, Hex outlineColor) {
    if (outline > 0) {
        DrawRectangleRec({rec.x - outline * m_Scale, rec.y - outline * m_Scale,
                          rec.width + outline * m_Scale * 2, rec.height + outline * m_Scale * 2}, 
                          GetColor(outlineColor));
    }

    DrawRectangleRec(rec, GetColor(fillColor));
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

    if (tokens.at(0) == "nop") {
        return;
    }
}
