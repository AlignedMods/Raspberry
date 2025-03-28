#include "game.hpp"

#include <pch.hpp>

#include "collectables/collectable.hpp"
#include "core/core.hpp"
#include "core/random.hpp"
#include "entity/player.hpp"
#include "graphics/renderer.hpp"
#include "menu/menu.hpp"

// Use a c++ wrapper for raylib which purposfully hurts the performance and
// makes the code more unreadeable 
#include "raylib-cpp.hpp"

using std::make_shared;
using std::shared_ptr;

static Game* s_Instance;
static shared_ptr<Renderer> s_Renderer;

Game::Game() {
  if (!s_Instance) {
    s_Renderer = make_shared<Renderer>();
    s_Instance = this;
  } else {
    Error(
        "Game class already exists!\nThere cannot be multiple game classes "
        "at the same time!",
        1);
  }
  SetCurrentLevel(make_shared<Level>());
}

Game::~Game() {}

void Game::Run() {
  // If this macro is specified then we will search for the Assets directory one
  // directory at a time until we find it
#ifdef RTRACEASSETS
  if (!std::filesystem::exists(std::filesystem::current_path() / "Assets")) {
    while (
        !std::filesystem::exists(std::filesystem::current_path() / "Assets")) {
      std::filesystem::current_path(
          std::filesystem::current_path().parent_path());
    }

    std::cout << "FOUND THE DIRECTORY!\n";
  }
#endif

  constexpr int WINDOW_WIDTH = 21 * 64;
  constexpr int WINDOW_HEIGHT = 12 * 64;

  raylib::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Raspberry");

  Random::Init();

  window.SetTargetFPS(60);

  Menu menu;
  menu.InitFonts();

  // for some reason i have to explictly show the menu
  menu.Show();

  raylib::Camera2D camera;
  camera.zoom = 1.0f;
  camera.offset = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
  camera.target = {0.0f, 0.0f};
  camera.rotation = 0.0f;

  while (!window.ShouldClose()) {
    if (m_IsGamePlayRunning) {
      menu.Hide();

      m_CurrentLevel->GetPlayer()->OnUpdate();

      camera.target = {m_CurrentLevel->GetPlayer()->GetX() * 64,
                       m_CurrentLevel->GetPlayer()->GetY() * 64};

      m_CurrentLevel->GetCollectable().Update();
    }

    menu.Update();

    window.BeginDrawing();

    window.ClearBackground(WHITE);

    window.DrawFPS();

    if (m_IsGamePlayRunning) {
      camera.BeginMode();

      for (auto& tile : m_CurrentLevel->GetAllTiles()) {
        tile.Draw();
      }

      m_CurrentLevel->GetPlayer()->OnRender();

      if (m_CurrentLevel->IsCollectableFound()) {
        DrawText("Found raspberry", 400, 600, 20, GREEN);
      }

      m_CurrentLevel->GetCollectable().Draw();

      camera.EndMode();
    }

    menu.Draw();

    window.EndDrawing();
  }
}

void Game::SetCurrentLevel(shared_ptr<Level> level) {
  m_CurrentLevel = level;
}

shared_ptr<Level> Game::GetCurrentLevel() {
  return Game::Get()->m_CurrentLevel;
}

void Game::StartGameplay() {
  m_IsGamePlayRunning = true;

  m_CurrentLevel->AddPlayer();

  m_CurrentLevel->AddCollectable();
  m_CurrentLevel->GetCollectable().InitTextures();

  m_CurrentLevel->LoadLevelFromFile("hi.lvl");
}

Game* Game::Get() {
  return s_Instance;
}

shared_ptr<Renderer> Game::GetRenderer() {
  return s_Renderer;
}
