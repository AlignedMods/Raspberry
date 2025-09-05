#include "game.hpp"
#include "application/application.hpp"
#include "entity/player.hpp"
#include "gui/menu.hpp"
#include "log.hpp"
#include "registry/registry.hpp"

#include "raylib.h"
#include "rlgl.h"
#include "imgui.h"

// no remorse

Game::Game() {
    SetName("game");
}

Game::~Game() {}

void Game::OnInit() {
    m_Camera.target = {0.0f, 0.0f};
    m_Camera.offset = {GetScreenWidth() / 2.0f - 16.0f, GetScreenHeight() / 2.0f - 16.0f};
    m_Camera.rotation = 0.0f;
    m_Camera.zoom = 1.0f;

    m_EditorCamera.target = {0.0f, 0.0f};
    m_EditorCamera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    m_EditorCamera.rotation = 0.0f;
    m_EditorCamera.zoom = 1.0f;

#ifdef RDEBUG
    SetExitKey(KEY_F9);
#else
    SetExitKey(0);
#endif

    m_StrResolutions = FormatResolutions();
    m_StrFramerates = FormatFramerates();

    m_Viewport = {m_Camera.target.x - m_Camera.offset.x / m_Camera.zoom, m_Camera.target.y - m_Camera.offset.y / m_Camera.zoom,
                  GetScreenWidth() / m_Camera.zoom, GetScreenHeight() / m_Camera.zoom};

    Application::Get().GetRegistry().AddArchive("core.rsp");

    InitMenu();

    Log(Log_Info, "Game from on init: %p", this);
}

void Game::OnUpdate(f32 ts) {
    // Viewport should always look the same
    m_Camera.zoom = 1.0f / (360.0f / GetScreenHeight());

    m_Viewport = {m_Camera.target.x - m_Camera.offset.x / m_Camera.zoom, m_Camera.target.y - m_Camera.offset.y / m_Camera.zoom,
                  GetScreenWidth() / m_Camera.zoom, GetScreenHeight() / m_Camera.zoom};

    // if (GetScreenWidth() % 640 || GetScreenHeight() % 360) { Log(LogLevel::Warning, "Use of unsupported window size!\nYou will likely run into issues!"); }

    // Fullscreen keybind
    if (IsKeyPressed(KEY_F11)) {
        SetFullscreen(!IsWindowFullscreen());
        mt_Fullscreen = IsWindowFullscreen();
    }

    if (m_GameRunning && !m_Paused) {
        m_Camera.offset = {GetScreenWidth() / 2.0f - 16.0f * m_Camera.zoom, GetScreenHeight() / 2.0f - 16.0f * m_Camera.zoom};

        if (!m_Paused) {
            // m_CurrentLevel->OnUpdate();
        }

        // m_Camera.target = {m_CurrentLevel->GetPlayer().GetX() * 32.0f,
        //                    m_CurrentLevel->GetPlayer().GetY() * 32.0f};
    }

    if (IsGamepadAvailable(0)) {
        SetGamepadVibration(0, 0.2f, 0.2f, 10.0f);
    }

    // Renderer.RenderText("HeLlO wOrLd!", {200.0f, 100.0f}, 20);

    // Gui.OnUpdate();
}

void Game::OnFixedUpdate() {
    if (m_GameRunning && !m_Paused) {
    }

    // m_CurrentFPS = 1.0f / Application::Get();
}

void Game::OnRender() {
    if (m_GameRunning) {
        BeginMode2D(m_Camera);

        for (auto& tile : m_CurrentLevel->tiles) {
            DrawTexturePro(tile.texture, {0, 0, static_cast<float>(tile.texture.width), static_cast<float>(tile.texture.height)}, {tile.pos.x * 32.0f, tile.pos.y * 32.0f, 32, 32}, {0, 0}, 0.0f, WHITE);
        }

        EndMode2D();
    }

    RenderCurrentMenu();

    // Gui.OnRender();

    // show the current fps
    // DrawText(TextFormat("FPS: %.1f", m_CurrentFPS), 10, 10, 20, GREEN);
    // DrawText(TextFormat("Time took to render: %.4f ms", deltaTime * 1000.0f), 10, 40, 20, GREEN);
}

static bool doLog = false;

void Game::OnUIRender(f32 ts) {}

void Game::OnEvent(const Event& event) {
    UpdateCurrentMenu(event);
}

const Camera2D& Game::GetCamera() const {
    return m_Camera;
}

const Rectangle& Game::GetViewportRect() const {
    return m_Viewport;
}

constexpr std::string Game::FormatResolutions() {
    std::string buffer;

    for (auto& res : m_Resolutions) {
        buffer.append(std::to_string((int)res.x));
        buffer.append(1, 'x');
        buffer.append(std::to_string((int)res.y));
        buffer.append(1, ';');
    }

    return buffer;
}

constexpr std::string Game::FormatFramerates() {
    std::string buffer;

    for (auto& fr : m_Framerates) {
        buffer.append(std::to_string((int)fr));
        buffer.append(1, ';');
    }

    return buffer;
}

void Game::SetFullscreen(bool yesno) {
}

void Game::SetPause(bool yes) {
    m_Paused = yes;
}

void Game::SetCurrentLevel(const Level& level) {
    // m_CurrentLevel = level;
}

Level* Game::GetCurrentLevel() {
    return m_CurrentLevel;
}

void Game::StartGameplay() {
    Log(Log_Info, "yeah");
    if (m_GameRunning) {
        return;
    }

    m_GameRunning = true;

    Log(Log_Info, "Game from start gay: %p", this);

    m_CurrentLevel = &(Application::Get().GetRegistry().GetLevel("hi"));

    // m_CurrentMenu = Menu::None;
}
