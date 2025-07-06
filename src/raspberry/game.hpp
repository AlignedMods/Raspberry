#pragma once

#include "raspberry/renderer/renderer.hpp"
#include "raspberry/level/level.hpp"
#include "raspberry/menu/menu.hpp"

#include <memory>

class s_Game {
public:
    s_Game();
    ~s_Game();

    void Run();
    void Loop(Menu& menu, Camera2D& camera);
    void Tick();

    void SetCurrentLevel(std::shared_ptr<Level> level);

    void StartGameplay();

	std::shared_ptr<Level> GetCurrentLevel();

private:
	std::shared_ptr<Level> m_CurrentLevel = nullptr;

    bool m_isGamePlayRunning = false;

	int targetFPS = 120;
  
public:
    float m_LastTime;
    float m_Time = 0.0f;

	float deltaTime = 0.0f;
};

inline s_Game Game;
