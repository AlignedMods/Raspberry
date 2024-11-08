#include "button.hpp"
#include "raylib.h"

#define TESTING 0

Button::Button(Menu* menu, int x, int y, const char* txt) {
	m_X = x;
	m_Y = y;
	m_Txt = txt;

	m_Size = {200, 60};

	m_Menu = menu;
}

void Button::Draw() {
	const Color black = {0x00, 0x00, 0x00, 0xff};
	DrawText(m_Txt, (float)m_X, (float)m_Y, 60, BLACK);
}

bool Button::IsPressed() {
	Rectangle rec = {m_X, m_Y, m_Size.x, m_Size.y};	

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), rec)) {
		return true;
	}

	return false;
}
