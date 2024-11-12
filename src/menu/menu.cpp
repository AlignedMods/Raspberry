#include "menu.hpp"
#include "game.hpp"

#include "pch.hpp"

Menu::Menu()
{
    m_Button = new Button(this, 500, 300, "Start");
    std::cout << "Created button!";
}

void Menu::InitFonts()
{
    m_Font = LoadFont("Assets/Fonts/lato-regular.ttf");
}

void Menu::Update()
{
    if (m_Button->IsPressed()) {
        Game::Get()->StartGameplay();
    }
}

void Menu::Draw()
{
    if (!m_Hidden) {
        m_Button->Draw();
    }
}

Font& Menu::GetFont()
{
    return m_Font;
}

void Menu::Hide()
{
    m_Hidden = true;
}

void Menu::Show()
{
    m_Hidden = false;
}
