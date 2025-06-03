#include "menu.hpp"
#include "game.hpp"

#include "pch.hpp"

Menu::Menu() {
    AddButton(std::make_shared<Button>(Vec2d(0.25f, 0.25f), Vec2d(0.5f, 0.5f), "Start", true, []() {
        Game::Get()->StartGameplay();
    }));
    std::cout << "Created button!";
}

void Menu::InitFonts() {
    m_Font = LoadFont("Assets/Fonts/lato-regular.ttf");
}

void Menu::Update() {
    for (auto& button : m_Buttons) {
        button->OnUpdate();
    }
}

void Menu::Draw() {
    if (!m_Hidden) {
        for (auto& button : m_Buttons) {
            button->OnRender();
        }
    }
}

Font& Menu::GetFont() {
    return m_Font;
}

void Menu::AddButton(std::shared_ptr<Button> button) {
    m_Buttons.push_back(button);
}

void Menu::Hide() {
    m_Hidden = true;
}

void Menu::Show() {
    m_Hidden = false;
}
