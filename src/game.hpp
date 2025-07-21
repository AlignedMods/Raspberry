#pragma once

#include "editor/editor.hpp"
#include "renderer/renderer.hpp"
#include "level/level.hpp"

#include "raylib.h"

#include <cmath>
#include <cstddef>

struct ApplicationSpecification {
	const char* Name = "Raspberry";

	// The normal size for the window is 720p
	int Width = 1280;
	int Height = 720;

	// The max size for the window is 4K
	int MaxWidth = 3840;
	int MaxHeight = 2160;

	// The min size for the window is 360p 
	int MinWidth = 640;
	int MinHeight = 360;
};

enum class Menu {
    None = 0,
    Main,
    Pause,
    Quit
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

    // Pause will automatically resume if the game is already paused
    void Pause();

    void Quit();

	Level& GetCurrentLevel();

private:
    void UpdateUI();

private:
	Level m_CurrentLevel;
	Editor* m_Editor;

    // Viewports
	Camera2D m_Camera;
	Camera2D m_EditorCamera;

	// NOTE: setting this to false will close the game!
	bool m_Running = true;

    bool m_GameRunning = false;
	bool m_EditorRunning = false;

	bool m_Paused = false;
    
	int targetFPS = 360;
	float m_CurrentFPS = 0;

	ApplicationSpecification m_Specification;

    Menu m_CurrentMenu = Menu::Main;
    // useful for menu's that go back to the previous one
    Menu m_PreviousMenu = Menu::None;
  
public:

	float deltaTime = 0.0f;

    size_t ticks = 0;
};

inline s_Game Game;
