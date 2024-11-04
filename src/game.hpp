#pragma once

#include "level/level.hpp"
#include "graphics/renderer.hpp"

class Game {
public:
	Game();
	~Game();

	void Run();

	void SetCurrentLevel(Level* level);
	Level* GetCurrentLevel();

	void StartGameplay();
	
	static Game* Get();
	static Renderer* GetRenderer();
private:
	Level* m_CurrentLevel = nullptr;

	bool m_IsGamePlayRunning = false;
};
