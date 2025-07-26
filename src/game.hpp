#pragma once

#include "editor/editor.hpp"
#include "level/level.hpp"

#include "raylib.h"

#include <array>
#include <cstddef>
#include <cstdint>

enum class Menu {
    None = 0,
    Main,
    Editor,
    Pause,
    Settings,
    Quit
};

// a submenu works together along with a menu
enum class SubMenu {
    None,
    Video
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
    constexpr std::string FormatResolutions();
    constexpr std::string FormatFramerates();
    void SetFullscreen(bool yesno);

private:
	Level m_CurrentLevel;
	Editor* m_Editor;

    // -- VIEWPORTS -- //
	Camera2D m_Camera;
	Camera2D m_EditorCamera;

    // -- APPLICATION -- //
    
	bool m_Running = true;

	int m_TargetFPS = 60;
	float m_CurrentFPS = 0;

    int m_PreviousWindowWidth, m_PreviousWindowHeight;

    // -- GAME -- //

    bool m_GameRunning = false;
	bool m_EditorRunning = false;

	bool m_Paused = false;
    
    // -- GUI -- //

    Menu m_CurrentMenu = Menu::Main;
    SubMenu m_CurrentSubMenu = SubMenu::None;
    Menu m_PreviousMenu = Menu::None;

    std::array<Vector2, 20> m_Resolutions = {{
        // yes i had to write this out manually
        {640,  360}, 
        {640,  480},
        {800,  600}, 
        {1024, 768}, 
        {1280, 720}, 
        {1280, 800}, 
        {1280, 1024}, 
        {1360, 768}, 
        {1440, 900}, 
        {1536, 864}, 
        {1600, 900}, 
        {1600, 1200}, 
        {1680, 1050}, 
        {1920, 1080}, 
        {1920, 1200}, 
        {2560, 1080}, 
        {2560, 1440}, 
        {2560, 1600}, 
        {3440, 1440}, 
        {3840, 2160} 
    }};

    std::array<uint32_t, 9> m_Framerates = {{
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
    
    uint32_t mt_TargetFPS = 60;
    uint32_t mt_FPSCap = 1;

    uint32_t mt_ResolutionIndex = 4;
    uint32_t mt_Fullscreen = 0;
  
public:
	float deltaTime = 0.0f;
    size_t ticks = 0;
};

inline s_Game Game;
