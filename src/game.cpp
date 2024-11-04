#include "game.hpp"
#include "core/core.hpp"
#include "graphics/renderer.hpp"
#include "menu/menu.hpp"
#include "raylib.h"
#include "player/player.hpp"
#include "tile/tile.hpp"

static Game* s_Instance = nullptr;
static Renderer* s_Renderer = nullptr;

Game::Game() {
	if (!s_Instance) {
		s_Renderer = new Renderer();
		s_Instance = this;
	} else {
		Error("Game class already exists!\nThere cannot be multiple game classes at the same time!", 1);
	}
	SetCurrentLevel(new Level());
}

Game::~Game() {
	delete m_CurrentLevel;
}

void Game::Run() {
	const int windowWidth = 21 * 64;
	const int windowHeight = 12 * 64;

	InitWindow(windowWidth, windowHeight, "Raspberry");

	SetTargetFPS(60);

	Menu menu;
	menu.InitFonts();

	// for some reason i have to explictly show the menu
	menu.Show();

	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.offset = {windowWidth / 2.0f, windowHeight / 2.0f};
	camera.target = {0.0f, 0.0f};
	camera.rotation = 0.0f;

	while(!WindowShouldClose()) {
		if (m_IsGamePlayRunning) {
			menu.Hide();

			m_CurrentLevel->GetPlayer()->Update();

			camera.target = {m_CurrentLevel->GetPlayer()->GetX(), m_CurrentLevel->GetPlayer()->GetY()};

			m_CurrentLevel->GetRaspberry()->Update();
		}

		menu.Update();

		BeginDrawing();

		ClearBackground(WHITE);

		if (m_IsGamePlayRunning) {
			BeginMode2D(camera);

			m_CurrentLevel->GetPlayer()->Draw();

			for (Tile tile : m_CurrentLevel->GetAllTiles()) {
				tile.Draw();	
			}

			if (m_CurrentLevel->IsRaspberryFound()) {
				DrawText("Found raspberry", 400, 600, 20, GREEN);
			}

			m_CurrentLevel->GetRaspberry()->Draw();

			EndMode2D();
		}

		menu.Draw();

		EndDrawing();
	}
}

void Game::SetCurrentLevel(Level* level) {
	m_CurrentLevel = level;
}

Level* Game::GetCurrentLevel() {
	return m_CurrentLevel;
}

void Game::StartGameplay() {
	m_IsGamePlayRunning = true;

	m_CurrentLevel->AddPlayer();

	m_CurrentLevel->LoadLevelFromFile("hi.lvl");

	m_CurrentLevel->AddRaspberry();
	m_CurrentLevel->GetRaspberry()->InitTextures();
}

Game* Game::Get() {
	return s_Instance;
}

Renderer* Game::GetRenderer() {
	return s_Renderer;
}
