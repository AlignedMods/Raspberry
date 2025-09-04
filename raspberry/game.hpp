#pragma once

#include "application/layer.hpp"
#include "gui/menu.hpp"
#include "registry/registry.hpp"

#include "raylib.h"

#include <array>

class Game : public Layer {
public:
    Game();
    ~Game();

    virtual void OnInit() override;
    virtual void OnUpdate(f32 ts) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnUIRender(f32 ts) override;
    virtual void OnEvent(const Event& event) override;

    void SetCurrentLevel(const Level& level);

    void StartGameplay();

    void SetPause(bool yes);

    Level* GetCurrentLevel();

    const Camera2D& GetCamera() const;
    const Rectangle& GetViewportRect() const;

private:
    void UpdateUI();
    constexpr std::string FormatResolutions();
    constexpr std::string FormatFramerates();
    void SetFullscreen(bool yesno);

public:
    bool m_GameRunning = false;

private:
    Level* m_CurrentLevel = nullptr;

    // -- VIEWPORTS -- //
    Camera2D m_Camera;
    Camera2D m_EditorCamera;

    Rectangle m_Viewport;

    // -- GAME -- //

    bool m_Paused = false;

    std::array<Vector2, 9> m_Resolutions = {{
        // yes i had to write this out manually
        {640, 360},
        {1280, 720},
        {1920, 1080},
        {2560, 1080},
        {2560, 1440},
        {3440, 1440},
        {3840, 2160},
        {5120, 2160},
        {7680, 4320} // 8k resolution, i do NOT know why you would ever need this (game runs like shit on this resolution)
    }};

    std::array<u32, 9> m_Framerates = {{60,
                                        75,
                                        120,
                                        144,
                                        165,
                                        240,
                                        360,
                                        480,
                                        720}};

    std::string m_StrResolutions;
    std::string m_StrFramerates;

    // -- TEMPORARY GUI STUFF -- //

    u32 mt_TargetFPS = 60;
    u32 mt_FPSCap = 1;

    u32 mt_ResolutionIndex = 0;
    u32 mt_Fullscreen = 0;
};
