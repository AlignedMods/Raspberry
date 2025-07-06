#pragma once

#include "raspberry/gui/raspGui.hpp"

#include "raylib.h"

#include <memory>
#include <vector>

class Button;

class Menu {
public:
	Menu();

	void InitFonts();

	void Update();
	void Draw();

	Font& GetFont();

    void AddButton(std::shared_ptr<Button> button);

	void Hide();
	void Show();
private:
	std::vector<std::shared_ptr<Button>> m_Buttons;
	Font m_Font;

	bool m_Hidden;
};
