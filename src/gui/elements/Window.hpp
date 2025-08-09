#pragma once

#include "GuiElement.hpp"
#include "gui/elements/Text.hpp"

#include <unordered_map>
#include <memory>

class Window : public GuiElement {
public:
    Window(const GuiInfo& info, const std::string& title);

    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    Rectangle m_Window;
    Rectangle m_TopBar;

    bool m_Dragged = false;

    Text m_Title;

    // a window is basically it's own menu
    std::unordered_map<std::string, std::shared_ptr<GuiElement>> m_Elements;
};
