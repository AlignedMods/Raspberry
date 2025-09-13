#include "game.hpp"
#include "application/application.hpp"
#include "entity/player.hpp"
#include "gui/menu.hpp"
#include "log.hpp"
#include "registry/registry.hpp"
#include "image/image.hpp"
#include "application/renderer.hpp"

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

    Blackberry::Image fox = Blackberry::LoadImageFromFile("fox.jpg");

    Blackberry::Renderer& renderer = Blackberry::Application::Get().GetRenderer();

    Blackberry::Application::Get().SetWindowIcon(fox);

    m_Fox = renderer.GenTexture(fox);
}

void Game::OnUpdate(f32 ts) {

}

void Game::OnFixedUpdate() {

}

static f32 multiply = 1.0f;

void Game::OnRender() {
    RenderCurrentMenu();

    Blackberry::Renderer& renderer = Blackberry::Application::Get().GetRenderer();

    renderer.Begin(RenderingMode::Triangles);

    renderer.UseTexture(m_Fox);

    renderer.SetColor({1.0f, 1.0f, 1.0f, 1.0f});

    renderer.TexCoord2f(0.0f, 0.0f);
    renderer.Vertex2f(100.0f, 400.0f);
    renderer.TexCoord2f(0.0f, multiply);
    renderer.Vertex2f(100.0f, 100.0f);
    renderer.TexCoord2f(multiply, multiply);
    renderer.Vertex2f(500.0f, 100.0f);

    // renderer.SetColor({0.0f, 0.0f, 1.0f, 1.0f});

    renderer.TexCoord2f(0.0f, 0.0f);
    renderer.Vertex2f(100.0f, 400.0f);
    renderer.TexCoord2f(multiply, multiply);
    renderer.Vertex2f(500.0f, 100.0f);
    renderer.TexCoord2f(multiply, 0.0f);
    renderer.Vertex2f(500.0f, 400.0f);

    renderer.End();

    // Gui.OnRender();

    // show the current fps
    // DrawText(TextFormat("FPS: %.1f", m_CurrentFPS), 10, 10, 20, GREEN);
    // DrawText(TextFormat("Time took to render: %.4f ms", deltaTime * 1000.0f), 10, 40, 20, GREEN);
}

static bool doLog = false;

void Game::OnUIRender(f32 ts) {
    ImGui::Begin("cool window");

    ImGui::SliderFloat("Amount of foox", &multiply, 1.0f, 50.0f);
    ImGui::Image(m_Fox.id, ImVec2(200.0f, 200.0f));

    ImGui::End();
}

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
