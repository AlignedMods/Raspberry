#pragma once

#include "button.hpp"
#include "raylib.h"

class Button;

class Menu {
public:
	Menu();

	void InitFonts();

	void Update();
	void Draw();

	Font& GetFont();

	void Hide();
	void Show();
private:
	Button* m_Button;
	Font m_Font;

	bool m_Hidden;
};
