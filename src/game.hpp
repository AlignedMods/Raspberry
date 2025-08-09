#pragma once

#include "editor/editor.hpp"
#include "level/level.hpp"

#include "raylib.h"

#include <array>

class s_Game {
public:
    s_Game();
    ~s_Game();

    bool Init();
    void Shutdown();
    bool Running();

    void PollEvents();
    void CalculateTiming();

    void OnUpdate();
    void FixedUpdate();
    void OnRender();

    void SetCurrentLevel(const Level& level);

    void StartGameplay();
    void StartEditor();

    // Pause will automatically resume if the game is already paused
    void Pause();

    void Quit();

	Level* GetCurrentLevel();
    Editor* GetEditor();

    const Camera2D& GetCamera() const;
    const Rectangle& GetViewportRect() const;

private:
    void UpdateUI();
    constexpr std::string FormatResolutions();
    constexpr std::string FormatFramerates();
    void SetFullscreen(bool yesno);

private:
	Level* m_CurrentLevel = nullptr;
	Editor* m_Editor;

    // -- VIEWPORTS -- //
	Camera2D m_Camera;
	Camera2D m_EditorCamera;

    Rectangle m_Viewport;

    // -- APPLICATION -- //
    
	bool m_Running = true;

	u32 m_TargetFPS = 0;
	f32 m_CurrentFPS = 0;

    u32 m_PreviousWindowWidth, m_PreviousWindowHeight;

    // -- TIMING -- //
    
    f64 m_CurrentTime = 0.0;
    f64 m_LastTime = 0.0;
    f64 m_UpdateDrawTime = 0.0;
    f64 m_WaitTime = 0.0;

    f64 m_TickTime = 0.0;

    // -- GAME -- //

    bool m_GameRunning = false;
	bool m_EditorRunning = false;

	bool m_Paused = false;
    
    std::array<Vector2, 9> m_Resolutions = {{
        // yes i had to write this out manually
        {640,  360}, 
        {1280, 720}, 
        {1920, 1080}, 
        {2560, 1080}, 
        {2560, 1440}, 
        {3440, 1440}, 
        {3840, 2160},
        {5120, 2160},
        {7680, 4320} // 8k resolution, i do NOT know why you would ever need this (game runs like shit on this resolution)
    }};

    std::array<u32, 9> m_Framerates = {{
        60,
        75,
        120,
        144,
        165,
        240,
        360,
        480,
        720
    }};

    std::string m_StrResolutions;
    std::string m_StrFramerates;
    
    // -- TEMPORARY GUI STUFF -- //
    
    u32 mt_TargetFPS = 60;
    u32 mt_FPSCap = 1;

    u32 mt_ResolutionIndex = 0;
    u32 mt_Fullscreen = 0;
  
public:
	f32 deltaTime = 0.0f;
    sz ticks = 0;
};

extern s_Game Game;
