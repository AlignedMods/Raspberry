#pragma once

#include "renderer/renderer.hpp"
#include "level/level.hpp"
#include "menu/menu.hpp"

class s_Game {
public:
    s_Game();
    ~s_Game();

    void Run();
    void Loop(Camera2D& camera);
    void Tick();

    void SetCurrentLevel(const Level& level);

    void StartGameplay();

	Level& GetCurrentLevel();

private:
	Level m_CurrentLevel;

    bool m_isGamePlayRunning = false;

	int targetFPS = 360;
  
public:
    float m_LastTime;
    float m_Time = 0.0f;

	float deltaTime = 0.0f;
};

inline s_Game Game;
