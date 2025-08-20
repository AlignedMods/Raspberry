#include "game.hpp"
#include "entity/player.hpp"
#include "log.hpp"
#include "gui/menu.hpp"
#include "registry/registry.hpp"

#include "raylib.h"
#include "rlgl.h"

#include <cstdlib>

static s_Game* s_Instance = nullptr;

// no remorse

s_Game::s_Game() {
    if (!s_Instance) {
        s_Instance = this;
    } else {
        Log(Log::Critical, "There cannot be multiple game classes at once!!");
        exit(1);
    }
}

s_Game::~s_Game() {}

bool s_Game::Init() {
    SetTraceLogLevel(LOG_WARNING); // don't fucking display your shit ass messages (jk love you raysan :3)
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(640, 360, "Raspberry");

    // we can't create the window
    if (!IsWindowReady()) return false;

    Log(Log::Info, "Created window with name \"%s\"!", "Raspberry");

    InitAudioDevice();

    // there won't be any audio
    if (!IsAudioDeviceReady()) return false;

    Log(Log::Info, "Initialized audio device!");

    SetWindowIcon(LoadImage("Assets/Textures/stone.png"));
    SetWindowMinSize(640, 360);

    m_Camera.target = { 0.0f, 0.0f };
    m_Camera.offset = { GetScreenWidth() / 2.0f - 16.0f, GetScreenHeight() / 2.0f - 16.0f };
    m_Camera.rotation = 0.0f;
    m_Camera.zoom = 1.0f;

    m_EditorCamera.target = { 0.0f, 0.0f };
    m_EditorCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    m_EditorCamera.rotation = 0.0f;
    m_EditorCamera.zoom = 1.0f;

#ifdef RDEBUG
    SetExitKey(KEY_F9);
#else
    SetExitKey(0);
#endif

    m_StrResolutions = FormatResolutions();
    m_StrFramerates = FormatFramerates();

    m_LastTime = GetTime();

    m_Viewport = {m_Camera.target.x - m_Camera.offset.x / m_Camera.zoom, m_Camera.target.y - m_Camera.offset.y / m_Camera.zoom,
                  GetScreenWidth() / m_Camera.zoom, GetScreenHeight() / m_Camera.zoom};

    Registry.RegisterArchive("core.rsp");

    InitMenu();

    return true;
}

void s_Game::Shutdown() {
    CloseAudioDevice();
    CloseWindow();
}

bool s_Game::Running() {
    return m_Running;
}

void s_Game::PollEvents() {
    PollInputEvents();
}

void s_Game::CalculateTiming() {
    m_CurrentTime = GetTime();

    m_UpdateDrawTime = m_CurrentTime - m_LastTime;

    if (m_TargetFPS > 0) {
        m_WaitTime = (1.0 / static_cast<double>(m_TargetFPS)) - m_UpdateDrawTime;
        WaitTime(m_WaitTime);
        m_CurrentTime = GetTime();
        deltaTime = static_cast<float>(m_CurrentTime - m_LastTime);
    } else {
        deltaTime = static_cast<float>(m_UpdateDrawTime);
    }

    m_LastTime = m_CurrentTime;
}

void s_Game::OnUpdate() {
    // check if we should continue
    m_Running = m_Running && !WindowShouldClose();

    // Viewport should always look the same
    m_Camera.zoom = 1.0f / (360.0f / GetScreenHeight());

    m_Viewport = {m_Camera.target.x - m_Camera.offset.x / m_Camera.zoom, m_Camera.target.y - m_Camera.offset.y / m_Camera.zoom,
                  GetScreenWidth() / m_Camera.zoom, GetScreenHeight() / m_Camera.zoom};

    // if (GetScreenWidth() % 640 || GetScreenHeight() % 360) { Log(LogLevel::Warning, "Use of unsupported window size!\nYou will likely run into issues!"); }

    while (GetTime() - m_TickTime > 0.0167f) {
        m_TickTime += 0.0167f;

        FixedUpdate();
    }

    // Fullscreen keybind
    if (IsKeyPressed(KEY_F11)) {
        SetFullscreen(!IsWindowFullscreen());
        mt_Fullscreen = IsWindowFullscreen();
    }

    if (m_GameRunning && !m_Paused) {
        m_Camera.offset = { GetScreenWidth() / 2.0f - 16.0f * m_Camera.zoom, GetScreenHeight() / 2.0f - 16.0f * m_Camera.zoom };

        if (!m_Paused) {
            m_CurrentLevel->OnUpdate();
        }

        m_Camera.target = {m_CurrentLevel->GetPlayer().GetX() * 32.0f,
                           m_CurrentLevel->GetPlayer().GetY() * 32.0f};
    }

    UpdateCurrentMenu();

    if (IsGamepadAvailable(0)) {
        SetGamepadVibration(0, 0.2f, 0.2f, 10.0f);
    }

    // Renderer.RenderText("HeLlO wOrLd!", {200.0f, 100.0f}, 20);

    // Gui.OnUpdate();
}

void s_Game::FixedUpdate() {
    if (m_GameRunning && !m_Paused) {
        m_CurrentLevel->Tick();
    }

    ticks++;

    m_CurrentFPS = 1.0f / deltaTime;
}

void s_Game::OnRender() {
    if (m_GameRunning) {
        BeginMode2D(m_Camera);

        m_CurrentLevel->OnRender();

        EndMode2D();
    }

    RenderCurrentMenu();

    // Gui.OnRender();

    // show the current fps
    DrawText(TextFormat("FPS: %.1f", m_CurrentFPS), 10, 10, 20, GREEN);
    DrawText(TextFormat("Time took to render: %.4f ms", deltaTime * 1000.0f), 10, 40, 20, GREEN);
}

const Camera2D& s_Game::GetCamera() const {
    return m_Camera;
}

const Rectangle& s_Game::GetViewportRect() const {
    return m_Viewport;
}

constexpr std::string s_Game::FormatResolutions() {
    std::string buffer;

    for (auto& res : m_Resolutions) {
        buffer.append(std::to_string((int)res.x));
        buffer.append(1, 'x');
        buffer.append(std::to_string((int)res.y));
        buffer.append(1, ';');
    }

    return buffer;
}

constexpr std::string s_Game::FormatFramerates() {
    std::string buffer;

    for (auto& fr : m_Framerates) {
        buffer.append(std::to_string((int)fr));
        buffer.append(1, ';');
    }

    return buffer;
}

void s_Game::SetFullscreen(bool yesno) {
    int monitor = GetCurrentMonitor();

    // Toggling fullscreen will *NOT* automatically set the window's size
    // so we must so it manually (using borderless windowed fixes this however)
    if (IsWindowFullscreen() && yesno) {
        return;
    } else if (!IsWindowFullscreen() && !yesno) {
        return;
    }

    if (IsWindowFullscreen()) {
        Log(Log::Info, "Window is fullscreen!");

        SetWindowSize(m_PreviousWindowWidth, m_PreviousWindowHeight);

        m_PreviousWindowWidth = GetScreenWidth();
        m_PreviousWindowHeight = GetScreenHeight();

        ToggleFullscreen();

        return;
    } else {
        Log(Log::Info, "Window is NOT fullscreen!");
        m_PreviousWindowWidth = GetScreenWidth();
        m_PreviousWindowHeight = GetScreenHeight();

        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));

        ToggleFullscreen();

        return;
    }
}

void s_Game::SetPause(bool yes) {
    m_Paused = yes;
}

void s_Game::Quit() {
    m_Running = false;
}

void s_Game::SetCurrentLevel(const Level& level) {
    // m_CurrentLevel = level;
}

Level* s_Game::GetCurrentLevel() {
    return m_CurrentLevel;
}

void s_Game::StartGameplay() {
    if (m_GameRunning) {
        return;
    }

    m_GameRunning = true;

    m_CurrentLevel = &Registry.GetLevel("hi");

    //m_CurrentMenu = Menu::None;
}
