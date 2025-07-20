#pragma once

enum class MenuTypes {
    None = 0,
    Main,
    Settings,
    Pause
};

class Menu {
public:
    void OnUpdate();

    void SetCurrentMenu(MenuTypes menu);

private:
    MenuTypes m_Menu = MenuTypes::Main;
};
