#include "game.hpp"
#include "core/random.hpp"
#include "entity/player.hpp"
#include "core/log.hpp"
#include "gui/raspGui.hpp"
#include "registry.hpp"

#include "raylib.h"
#include "rlgl.h"

#include <cstdlib>

static s_Game* s_Instance = nullptr;

// no remorse

s_Game::s_Game() {
    if (!s_Instance) {
        s_Instance = this;
    } else {
        Log(LogLevel::Critical, "There cannot be multiple game classes at once!!");
        exit(1);
    }
}

s_Game::~s_Game() {}

bool s_Game::Init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(640, 360, "Raspberry");

    // we can't create the window
    if (!IsWindowReady()) return false;

    InitAudioDevice();

    // there won't be any audio
    if (!IsAudioDeviceReady()) return false;

    SetTraceLogLevel(LOG_WARNING);

    Registry.RegisterAllTextures();

    SetWindowIcon(LoadImage("Assets/Textures/stone.png"));
    SetWindowMinSize(640, 360);

    m_Camera.target = { 0.0f, 0.0f };
    m_Camera.offset = { GetScreenWidth() / 2.0f - 64.0f, GetScreenHeight() / 2.0f - 64.0f };
    m_Camera.rotation = 0.0f;
    m_Camera.zoom = 1.0f;

    m_EditorCamera.target = { 0.0f, 0.0f };
    m_EditorCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    m_EditorCamera.rotation = 0.0f;
    m_EditorCamera.zoom = 1.0f;

    Random::Init();

#ifdef RDEBUG
    SetExitKey(KEY_F9);
#else
    SetExitKey(0);
#endif

    m_StrResolutions = FormatResolutions();
    m_StrFramerates = FormatFramerates();

    m_LastTime = GetTime();

    m_CurrentLevel = Level();

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
    m_Camera.zoom = 0.5f / (360.0f / GetScreenHeight());

    m_Viewport = {m_Camera.target.x - m_Camera.offset.x / m_Camera.zoom, m_Camera.target.y - m_Camera.offset.y / m_Camera.zoom,
                  GetScreenWidth() / m_Camera.zoom, GetScreenHeight() / m_Camera.zoom};

    while (GetTime() - m_TickTime > 0.0167f) {
        m_TickTime += GetTime() - m_TickTime;

        FixedUpdate();
    }

    RaspGui::NewCanvas();

    // Fullscreen keybind
    if (IsKeyPressed(KEY_F11)) {
        SetFullscreen(!IsWindowFullscreen());
        mt_Fullscreen = IsWindowFullscreen();
    }

    if (m_GameRunning) {
        m_Camera.offset = { GetScreenWidth() / 2.0f - 32.0f * m_Camera.zoom, GetScreenHeight() / 2.0f - 32.0f * m_Camera.zoom };

        if (!m_Paused) {
            m_CurrentLevel.OnUpdate();
        }

        m_Camera.target = {m_CurrentLevel.GetPlayer().GetX() * 64.0f,
            m_CurrentLevel.GetPlayer().GetY() * 64.0f};

        if (IsKeyPressed(KEY_ESCAPE)) {
            Pause();
        }
    }

    if (m_EditorRunning) {
        m_EditorCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        m_Editor->OnUpdate();
    }

    UpdateUI();
}

void s_Game::FixedUpdate() {
    if (m_GameRunning && !m_Paused) {
        m_CurrentLevel.Tick();
    }

    ticks++;

    m_CurrentFPS = 1.0f / deltaTime;
}

void s_Game::OnRender() {
    if (m_GameRunning) {
        BeginMode2D(m_Camera);

        m_CurrentLevel.OnRender();

        if (m_CurrentLevel.IsCollectableFound()) {
            DrawText("Found raspberry", 400, 600, 20, GREEN);
        }

        EndMode2D();
    }

    if (m_EditorRunning) {
        m_Editor->OnRender();
    }

    RaspGui::Render();

    // show the current fps
    DrawText(TextFormat("FPS: %.1f", m_CurrentFPS), 10, 10, 20, GREEN);
    DrawText(TextFormat("Time took to render: %.4f ms", deltaTime * 1000.0f), 10, 40, 20, GREEN);
}

void s_Game::UpdateUI() {
    if (m_CurrentMenu == Menu::None) { return; }

    if (m_CurrentMenu == Menu::Main) {
        if (RaspGui::Button({10, 200, 100, 30}, "Play")) {
            StartGameplay();
        }

        if (RaspGui::Button({10, 240, 100, 30}, "Editor")) {
            StartEditor();
            m_CurrentMenu = Menu::Editor;
        }

        if (RaspGui::Button({10, 280, 100, 30}, "Settings")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Settings;
            m_CurrentSubMenu = SubMenu::Video;
        }

        if (RaspGui::Button({10, 320, 100, 30}, "Quit")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Quit;
        }

        RaspGui::SliderIntEx({100, 100, 200, 25}, &guy, 0, 1000, 20, RaspGui::GetPallete());

        RaspGui::Text({100, 140, 200, 25}, TextFormat("value: %d", guy));

        RaspGui::Label({30, 20, 590, 40}, "Welcome to raspberry!");

        RaspGui::BeginSquareMode();
        RaspGui::Label({200, 200, 40, 40}, "W!");
        RaspGui::EndSquareMode();
    }

    if (m_CurrentMenu == Menu::Pause) {
        if (RaspGui::Button({20, 180, 70, 30}, "Continue")) {
            Pause();
        }

        if (RaspGui::Button({20, 220, 70, 30}, "Settings")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Settings;
        }

        if (RaspGui::Button({20, 260, 70, 30}, "Main Menu")) {
            m_GameRunning = false;
            m_CurrentMenu = Menu::Main;
        }

        if (RaspGui::Button({20, 300, 70, 30}, "Quit")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Quit;
        }
    }

    if (m_CurrentMenu == Menu::Settings) {
        // Sidebar
        RaspGui::Panel({60, 30, 90, 300});

        if (RaspGui::Button({5, 5, 80, 40}, "Video")) {
            m_CurrentSubMenu = SubMenu::Video;
        }

        if (RaspGui::Button({10, 275, 70, 20}, "Back")) {
            m_CurrentMenu = m_PreviousMenu;
        }

        RaspGui::End();

        RaspGui::Panel({150, 30, 430, 300});

        if (m_CurrentSubMenu == SubMenu::Video) {
            RaspGui::Text({10, 5, 90, 30}, "Resolution: ");
            RaspGui::Text({10, 45, 90, 30}, "Fullscreen: ");
            RaspGui::Text({10, 85, 90, 30}, "FPS Cap: ");
            RaspGui::Text({10, 125, 90, 30}, "Target FPS: ");

            RaspGui::ComboBox({270, 5, 120, 30}, m_StrResolutions, &mt_ResolutionIndex);
            RaspGui::ComboBox({270, 45, 120, 30}, "No;Yes", &mt_Fullscreen);
            RaspGui::ComboBox({270, 85, 120, 30}, "No;Yes", &mt_FPSCap);
            RaspGui::ComboBox({270, 125, 120, 30}, m_StrFramerates, &mt_TargetFPS);

            if (RaspGui::Button({340, 270, 80, 25}, "Apply")) {
                if ((GetScreenWidth() != m_Resolutions.at(mt_ResolutionIndex).x || GetScreenHeight() != m_Resolutions.at(mt_ResolutionIndex).y) && !mt_Fullscreen) {
                    SetWindowSize(m_Resolutions.at(mt_ResolutionIndex).x, m_Resolutions.at(mt_ResolutionIndex).y);

                    // Center the window
                    SetWindowPosition(GetMonitorWidth(GetCurrentMonitor()) / 2 - GetScreenWidth() / 2, GetMonitorHeight(GetCurrentMonitor()) / 2 - GetScreenHeight() / 2);
                }

                SetFullscreen(mt_Fullscreen);

                if (mt_FPSCap) {
                    m_TargetFPS = m_Framerates.at(mt_TargetFPS);
                } else {
                    // fps is uncapped
                    m_TargetFPS = 0;
                }
            }
        }

        RaspGui::End();

    }

    if (m_CurrentMenu == Menu::Quit) {
        RaspGui::Text({40, 20, 560, 60}, "Are you sure you wish to quit?");

        if (RaspGui::Button({260, 160, 120, 30}, "Yes")) {
            Quit();
        }

        if (RaspGui::Button({260, 200, 120, 30}, "No")) {
            m_CurrentMenu = m_PreviousMenu;
        }
    }
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
        Log(LogLevel::Info, "Window is fullscreen!");

        SetWindowSize(m_PreviousWindowWidth, m_PreviousWindowHeight);

        m_PreviousWindowWidth = GetScreenWidth();
        m_PreviousWindowHeight = GetScreenHeight();

        ToggleFullscreen();

        return;
    } else {
        Log(LogLevel::Info, "Window is NOT fullscreen!");
        m_PreviousWindowWidth = GetScreenWidth();
        m_PreviousWindowHeight = GetScreenHeight();

        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));

        ToggleFullscreen();

        return;
    }

    Log(LogLevel::Info, "No condition is met!");
}

void s_Game::Pause() {
    m_Paused = !m_Paused;

    if (m_Paused) {
        m_CurrentMenu = Menu::Pause;
    } else {
        m_CurrentMenu = Menu::None;
    }
}

void s_Game::Quit() {
    m_Running = false;
}

void s_Game::SetCurrentLevel(const Level& level) {
    m_CurrentLevel = level;
}

Level& s_Game::GetCurrentLevel() {
    return m_CurrentLevel;
}

void s_Game::StartEditor() {
    if (m_EditorRunning || m_GameRunning) {
        return;
    }

    m_EditorRunning = true;

    m_Editor = new Editor(m_EditorCamera);
}

void s_Game::StartGameplay() {
    if (m_GameRunning || m_EditorRunning) {
        return;
    }

    m_GameRunning = true;

    m_CurrentLevel.LoadLevelFromFile("hi.rsp");

    m_CurrentLevel.AddCollectable();
    m_CurrentLevel.GetCollectable().InitTextures();

    m_CurrentMenu = Menu::None;
}
