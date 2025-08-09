#include "Window.hpp"
#include "gui/elements/Text.hpp"
#include "gui/gui.hpp"
#include "raylib.h"

Window::Window(const GuiInfo& info, const std::string& title) :
    m_Title(info, title)
{
    m_Info = info;

    m_Window = Gui.GetRealSize(m_Info);
    m_TopBar = m_Window;

    m_TopBar.height = 20 * Gui.GetScale();

    m_Window.y += 20 * Gui.GetScale();

    // m_Title.SetPosition( {m_TopBar.x, m_TopBar.y} );
}

void Window::OnUpdate() {
    Vector2 mouse = Gui.GetMousePos();

    if (CheckCollisionPointRec(mouse, m_Window)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_Dragged = true;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && m_Dragged) {
        m_Dragged = false;
    }

    if (m_Dragged) {
        m_Window.x += GetMouseDelta().x;
        m_Window.y += GetMouseDelta().y;

        m_TopBar.x += GetMouseDelta().x;
        m_TopBar.y += GetMouseDelta().y;
    }

    for (auto&[name, element] : m_Elements) {
        element->OnUpdate();
    }
}

void Window::OnRender() {
    Gui.RenderRectangle(m_Window, 2, Colors::Black, Colors::Yellow);
    Gui.RenderRectangle(m_TopBar, 2, Colors::DGray, Colors::Yellow);

    for (auto&[name, element] : m_Elements) {
        element->OnRender();
    }
}
