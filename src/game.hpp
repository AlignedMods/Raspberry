#pragma once

#include "editor/editor.hpp"
#include "renderer/renderer.hpp"
#include "level/level.hpp"
#include "menu/menu.hpp"

#include "raylib.h"

class s_Game {
public:
    s_Game();
    ~s_Game();

    void Run();
    void Loop();
    void Tick();

    void SetCurrentLevel(const Level& level);

    void StartGameplay();
    void StartEditor();

	Level& GetCurrentLevel();

private:
	Level m_CurrentLevel;
	Editor* m_Editor;

	Camera2D m_Camera;
	Camera2D m_EditorCamera;

    bool m_GameRunning = false;
	bool m_EditorRunning = false;
	int targetFPS = 360;

	float m_CurrentFPS = 0;
  
public:
    float m_LastTime;
    float m_Time = 0.0f;

	float deltaTime = 0.0f;
};

inline s_Game Game;
