#include "raspberry.hpp"
#include "raylib.h"
#include "game.hpp"

#include "pch.hpp"

Raspberry::Raspberry() {
	std::srand(std::time(0));

	m_Pos.x = rand() % 25;
	m_Pos.y = rand() % 18;

//	for (auto tile : Game::Get()->GetAllTiles()) {
//		if (tile.GetPos().x == m_Pos.x) {
//			m_Pos.x++;
//		} else if (tile.GetPos().y == m_Pos.y) {
//			m_Pos.y++;
//		}
//	}

//	std::cout << "Position of rasp is: " << m_Pos.x * 64 << " " << m_Pos.y * 64 << "\n";
}

void Raspberry::InitTextures() {
	Image image = LoadImage("Assets/raspberry.png");

	m_Texture = LoadTextureFromImage(image);
	UnloadImage(image);
}

void Raspberry::Update() {
	OnUpdate();
}

void Raspberry::OnTouch() {
	Game::Get()->GetCurrentLevel()->SetFound();
}

void Raspberry::OnUpdate() {
	if (CheckCollisionRecs({Game::Get()->GetCurrentLevel()->GetPlayer()->GetX(), Game::Get()->GetCurrentLevel()->GetPlayer()->GetY(), 64, 64}, {m_Pos.x * 64, m_Pos.y * 64, 64, 64})) {
		OnTouch();
	}
}
