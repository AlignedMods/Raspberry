#include "game.hpp"
#include "application/application.hpp"
#include "entity/player.hpp"
#include "gui/menu.hpp"
#include "log.hpp"
#include "registry/registry.hpp"

#include "imgui.h"

// no remorse

Game::Game() {
    SetName("game");
}

Game::~Game() {}

void Game::OnInit() {
    Blackberry::Application::Get().GetRegistry().AddArchive("core.rsp");

    InitMenu();

    Log(Log_Info, "Game from on init: %p", this);
}

void Game::OnUpdate(f32 ts) {

}

void Game::OnFixedUpdate() {

}

void Game::OnRender() {
    RenderCurrentMenu();

    // Gui.OnRender();

    // show the current fps
    // DrawText(TextFormat("FPS: %.1f", m_CurrentFPS), 10, 10, 20, GREEN);
    // DrawText(TextFormat("Time took to render: %.4f ms", deltaTime * 1000.0f), 10, 40, 20, GREEN);
}

static bool doLog = false;

void Game::OnUIRender(f32 ts) {}

void Game::OnEvent(const Blackberry::Event& event) {
    UpdateCurrentMenu(event);
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

    m_CurrentLevel = &(Blackberry::Application::Get().GetRegistry().GetLevel("hi"));

    // m_CurrentMenu = Menu::None;
}
