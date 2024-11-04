#pragma once

#include "menu.hpp"
#include "raylib.h"

class Menu;

class Button {
public:
	Button() {}
	Button(Menu* menu, int x, int y, const char* txt);
	
	void Draw();

	bool IsPressed();
private:
	float m_X;
	float m_Y;
	Vector2 m_Size;

	const char* m_Txt;

	Menu* m_Menu;
};
