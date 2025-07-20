#pragma once

#include "editor/editor.hpp"
#include "renderer/renderer.hpp"
#include "level/level.hpp"
#include "menu.hpp"

#include "raylib.h"

#include <cstddef>

struct ApplicationSpecification {
	const char* Name = "Raspberry";

	// The normal size for the window is 720p
	int Width = 1280;
	int Height = 720;

	// The max size for the window is 4K
	int MaxWidth = 3840;
	int MaxHeight = 2160;

	// The min size for the window is 480p 
	int MinWidth = 854;
	int MinHeight = 480;
};

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

    void Pause();
    void Resume();

    void Quit();

	Level& GetCurrentLevel();

private:
    void UpdateSettingsMenu();

private:
	Level m_CurrentLevel;
	Editor* m_Editor;

	Camera2D m_Camera;
	Camera2D m_EditorCamera;

    Menu m_Menu;

	// NOTE: setting this to false will close the game!
	bool m_Running = true;

    bool m_GameRunning = false;
	bool m_EditorRunning = false;
	bool m_PauseMenu = false;
	int targetFPS = 360;

	float m_CurrentFPS = 0;

	ApplicationSpecification m_Specification;

	Texture m_Cursor;
  
public:
    float m_LastTime;
    float m_Time = 0.0f;

	float deltaTime = 0.0f;

    size_t ticks = 0;
};

inline s_Game Game;
