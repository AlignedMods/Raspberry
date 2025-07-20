#include "menu.hpp"
#include "game.hpp"
#include "gui/raspGui.hpp"

void Menu::OnUpdate() {
    if (m_Menu == MenuTypes::Main) {
        if (RaspGui::Button({0.05f, 0.65f, 0.2f, 0.1f}, "Play")) {
            Game.StartGameplay();
            m_Menu = MenuTypes::None;
        }

        if (RaspGui::Button({0.05f, 0.8f, 0.2f, 0.1f}, "Editor")) {
            Game.StartEditor();
            m_Menu = MenuTypes::None;
        }

        RaspGui::Label({0.1f, 0.05f, 0.8f, 0.2f}, "Welcome to raspberry!");
    }

    if (m_Menu == MenuTypes::Pause) {
        if (RaspGui::Button({0.3f, 0.1f, 0.4f, 0.15f}, "Continue")) {
            m_Menu = MenuTypes::None;
        }

        if (RaspGui::Button({0.3f, 0.3f, 0.4f, 0.15f}, "Settings")) {
            m_Menu = MenuTypes::Settings;
        }

        if (RaspGui::Button({0.3f, 0.5f, 0.4f, 0.15f}, "Main Menu")) {
            Game.Pause();
            m_Menu = MenuTypes::Main;
        }

        if (RaspGui::Button({0.3f, 0.7f, 0.4f, 0.15f}, "Quit")) {
            Game.Quit();
        }
    }
}

void Menu::SetCurrentMenu(MenuTypes menu) {
    m_Menu = menu;
}
