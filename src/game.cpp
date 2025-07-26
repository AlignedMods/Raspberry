#include "game.hpp"
#include "core/random.hpp"
#include "entity/player.hpp"
#include "core/log.hpp"
#include "renderer/renderer.hpp"
#include "gui/raspGui.hpp"
#include "registry.hpp"

#include "raylib.h"
#include "rlgl.h"

static s_Game* s_Instance = nullptr;

// no remorse

s_Game::s_Game() {
    if (!s_Instance) {
        s_Instance = this;
    } else {
        Log(LogLevel::Critical, "There cannot be multiple game classes at once!!");
    }

    SetCurrentLevel(Level());

    SetTraceLogLevel(LOG_WARNING);
}

s_Game::~s_Game() {}

void s_Game::Loop() {
    double currentTime = 0.0;
    double lastTime = GetTime();
    double updateDrawTime = 0.0;
    double waitTime = 0.0;

    double tickTime = 0.0;

    while (!WindowShouldClose() && m_Running) {
        PollInputEvents();

        RaspGui::NewCanvas();

        while (GetTime() - tickTime > 0.0167f) {
            tickTime += GetTime() - tickTime;

            Tick();
        }

        // Fullscreen keybind
        if (IsKeyPressed(KEY_F11)) {
            SetFullscreen(!IsWindowFullscreen());
            mt_Fullscreen = IsWindowFullscreen();
        }

        if (m_GameRunning) {
            m_Camera.offset = { GetScreenWidth() / 2.0f - 64.0f, GetScreenHeight() / 2.0f - 64.0f };

            if (!m_Paused) {
                m_CurrentLevel.OnUpdate();
            }

            m_Camera.target = {m_CurrentLevel.GetPlayer().GetX() * 64.0f - 32.0f,
                m_CurrentLevel.GetPlayer().GetY() * 64.0f - 32.0f};

            if (IsKeyPressed(KEY_ESCAPE)) {
                Pause();
            }
        }

        if (m_EditorRunning) {
            m_EditorCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
            m_Editor->OnUpdate();
        }

        UpdateUI();

        // begin rendering
        Renderer.Begin();

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

        // show the current fps
        DrawText(TextFormat("FPS: %.1f", m_CurrentFPS), 10, 10, 20, GREEN);

        RaspGui::Render();

        Renderer.End();

        currentTime = GetTime();

        updateDrawTime = currentTime - lastTime;

        if (m_TargetFPS > 0) {
            waitTime = (1.0 / static_cast<double>(m_TargetFPS)) - updateDrawTime;
            WaitTime(waitTime);
            currentTime = GetTime();
            deltaTime = static_cast<float>(currentTime - lastTime);
        } else {
            deltaTime = static_cast<float>(updateDrawTime);
        }

        lastTime = currentTime;
    }
}

void s_Game::UpdateUI() {
    if (m_CurrentMenu == Menu::None) { return; }

    if (m_CurrentMenu == Menu::Main) {
        if (RaspGui::Button({0.05f, 0.55f, 0.15f, 0.05f}, "Play")) {
            StartGameplay();
        }

        if (RaspGui::Button({0.05f, 0.65f, 0.15f, 0.05f}, "Editor")) {
            StartEditor();
            m_CurrentMenu = Menu::Editor;
        }

        if (RaspGui::Button({0.05f, 0.75f, 0.15f, 0.05f}, "Settings")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Settings;
            m_CurrentSubMenu = SubMenu::Video;
        }

        if (RaspGui::Button({0.05f, 0.85f, 0.15f, 0.05f}, "Quit")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Quit;
        }

        RaspGui::Label({0.1f, 0.05f, 0.8f, 0.2f}, "Welcome to raspberry!");
    }

    if (m_CurrentMenu == Menu::Pause) {
        if (RaspGui::Button({0.3f, 0.1f, 0.4f, 0.15f}, "Continue")) {
            Pause();
        }

        if (RaspGui::Button({0.3f, 0.3f, 0.4f, 0.15f}, "Settings")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Settings;
        }

        if (RaspGui::Button({0.3f, 0.5f, 0.4f, 0.15f}, "Main Menu")) {
            m_GameRunning = false;
            m_CurrentMenu = Menu::Main;
        }

        if (RaspGui::Button({0.3f, 0.7f, 0.4f, 0.15f}, "Quit")) {
            m_PreviousMenu = m_CurrentMenu;
            m_CurrentMenu = Menu::Quit;
        }
    }

    if (m_CurrentMenu == Menu::Settings) {
        // Sidebar
        RaspGui::Panel({0.1f, 0.1f, 0.15f, 0.8f});

        if (RaspGui::Button({0.05f, 0.02f, 0.9f, 0.1f}, "Video")) {
            m_CurrentSubMenu = SubMenu::Video;
        }

        RaspGui::End();

        RaspGui::Panel({0.25f, 0.1f, 0.65f, 0.8f});

        if (m_CurrentSubMenu == SubMenu::Video) {
            RaspGui::Text({0.05f, 0.05f, 0.3f, 0.1f}, "Resolution: ");
            RaspGui::Text({0.05f, 0.20f, 0.3f, 0.1f}, "Fullscreen: ");
            RaspGui::Text({0.05f, 0.35f, 0.3f, 0.1f}, "FPS Cap: ");
            RaspGui::Text({0.05f, 0.50f, 0.3f, 0.1f}, "Target FPS: ");

            RaspGui::ComboBox({0.5f, 0.05f, 0.3f, 0.1f}, m_StrResolutions, &mt_ResolutionIndex);
            RaspGui::ComboBox({0.5f, 0.20f, 0.3f, 0.1f}, "No;Yes", &mt_Fullscreen);
            RaspGui::ComboBox({0.5f, 0.35f, 0.3f, 0.1f}, "No;Yes", &mt_FPSCap);
            RaspGui::ComboBox({0.5f, 0.50f, 0.3f, 0.1f}, m_StrFramerates, &mt_TargetFPS);

            if (RaspGui::Button({0.7f, 0.8f, 0.25f, 0.15f}, "Apply")) {
                if ((GetScreenWidth() != m_Resolutions.at(mt_ResolutionIndex).x || GetScreenHeight() != m_Resolutions.at(mt_ResolutionIndex).y) && !mt_Fullscreen) {
                    SetWindowSize(m_Resolutions.at(mt_ResolutionIndex).x, m_Resolutions.at(mt_ResolutionIndex).y);

                    // Center the window
                    SetWindowPosition(GetMonitorWidth(GetCurrentMonitor()) / 2 - GetScreenWidth() / 2, GetMonitorHeight(GetCurrentMonitor()) / 2 - GetScreenHeight() / 2);

                    // Viewport should always look the same
                    m_Camera.zoom = 1.0f / (720.0f / GetScreenHeight());
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

        if (RaspGui::Button({0.03f, 0.92f, 0.15f, 0.05f}, "Back")) {
            m_CurrentMenu = m_PreviousMenu;
        }
    }

    if (m_CurrentMenu == Menu::Quit) {
        RaspGui::Text({0.3f, 0.2f, 0.4f, 0.1f}, "Are you sure you wish to quit?");

        if (RaspGui::Button({0.4f, 0.4f, 0.2f, 0.1f}, "Yes")) {
            Quit();
        }

        if (RaspGui::Button({0.4f, 0.55f, 0.2f, 0.1f}, "No")) {
            m_CurrentMenu = m_PreviousMenu;
        }
    }
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

// NOTE: a tick is basically a 60th of a second.
// esentially acting as a "FixedUpdate"
// You *should* be able to always run this game on at least 60fps
// if you can't certain things will slow down
void s_Game::Tick() {
    if (m_GameRunning && !m_Paused) {
        m_CurrentLevel.Tick();
    }

    ticks++;

    m_CurrentFPS = 1.0f / deltaTime;
}

void s_Game::Run() {
    // If this macro is specified then we will search for the Assets directory one
    // directory at a time until we find it
#ifdef RTRACEASSETS
    if (!std::filesystem::exists(std::filesystem::current_path() / "Assets")) {
        while (
            !std::filesystem::exists(std::filesystem::current_path() / "Assets")) {
            std::filesystem::current_path(
                std::filesystem::current_path().parent_path());
        }

        Debug("FOUND THE DIRECTORY!");
    }
#endif

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Raspberry");

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

    Loop();
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
