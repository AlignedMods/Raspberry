//#include "menu.hpp"
//#include "game.hpp"
//
//Menu::Menu() {
//    AddButton(std::make_shared<Button>(Vector2(0.25f, 0.25f), Vector2(0.5f, 0.5f), 1.0f,"Start", true, []() {
//        Game.StartGameplay();
//    }));
//}
//
//void Menu::InitFonts() {
//    m_Font = LoadFont("Assets/Fonts/lato-regular.ttf");
//}
//
//void Menu::Update() {
//    for (auto& button : m_Buttons) {
//        button->OnUpdate();
//    }
//}
//
//void Menu::Draw() {
//    if (!m_Hidden) {
//        for (auto& button : m_Buttons) {
//            button->OnRender();
//        }
//    }
//}
//
//Font& Menu::GetFont() {
//    return m_Font;
//}
//
//void Menu::AddButton(std::shared_ptr<Button> button) {
//    m_Buttons.push_back(button);
//}
//
//void Menu::Hide() {
//    m_Hidden = true;
//}
//
//void Menu::Show() {
//    m_Hidden = false;
//}
