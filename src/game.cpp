#include "game.hpp"

#include "collectables/collectable.hpp"
#include "core/core.hpp"
#include "core/random.hpp"
#include "entity/player.hpp"
#include "graphics/renderer.hpp"
#include "menu/menu.hpp"

#include "raylib.h"
#include "utils/types.hpp"

#include "gui/raspGui.hpp"

#include <pch.hpp>

using std::make_shared;
using std::shared_ptr;

static Game* s_Instance;
static shared_ptr<Renderer> s_Renderer;

bool sorted = false;

bool CompareZOrder(std::shared_ptr<Instance> a, std::shared_ptr<Instance> b) {
    return (a->m_ZOrder < b->m_ZOrder);
}

void Callback() {
    Debug("Clicked");
}

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

  SetTraceLogLevel(LOG_WARNING);
}

Game::~Game() {}

void Game::Loop(Menu& menu, Camera2D& camera) {
    Button button = Button(Vec2d(0.25f, 0.25f), Vec2d(0.5f, 0.5f), "funny", true, []() {Debug("hi");});

    while (!WindowShouldClose()) {
        if (m_isGamePlayRunning) {
            m_Time += 0.0167f;

            while (GetTime() - m_LastTime > 0.05f) {
                Tick();

                m_LastTime += 0.05f;
            }
        
            menu.Hide();

            m_CurrentLevel->GetPlayer()->OnUpdate();

            //m_CurrentLevel->GetPlayer()->UpdateTextures();
        
            camera.target = {m_CurrentLevel->GetPlayer()->GetX(),
                            m_CurrentLevel->GetPlayer()->GetY()};
        
            m_CurrentLevel->GetCollectable().Update();
        }

        button.OnUpdate();
    
        menu.Update();
    
        BeginDrawing();

        button.OnRender();
    
        ClearBackground(WHITE);
    
        DrawFPS(5, 5);
    
        if (m_isGamePlayRunning) {
            BeginMode2D(camera);

            for (auto instance : m_CurrentLevel->GetInstances()) {
                if (!sorted) {
                    std::sort(m_CurrentLevel->GetInstances().begin(), m_CurrentLevel->GetInstances().end(), CompareZOrder);
                    sorted = true;
                }
                
                if (instance->m_ZOrder < 0.0f) {
                    instance->OnRender();
                }

                instance->OnRender();
            }
      
            //DrawRectangle(80, 90, 70, 200, BLACK);
      
            //m_CurrentLevel->GetPlayer()->Render();
      
            if (m_CurrentLevel->IsCollectableFound()) {
                DrawText("Found raspberry", 400, 600, 20, GREEN);
            }
      
            m_CurrentLevel->GetCollectable().Draw();
      
            EndMode2D();
        }
    
        menu.Draw();
    
        EndDrawing();
    }
}

// NOTE: a tick is basically a 20th of a second aka 3 frames.
void Game::Tick() {
    m_CurrentLevel->GetPlayer()->UpdateTextures();
}

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

    Debug("FOUND THE DIRECTORY!");
  }
#endif

    constexpr static int WINDOW_WIDTH = 21 * 64;
    constexpr static int WINDOW_HEIGHT = 12 * 64;
  
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raspberry");
  
    m_LastTime = GetTime();
  
    Random::Init();
  
    SetTargetFPS(60);

    //ToggleBorderlessWindowed();
    SetExitKey(KEY_F9);
  
    Menu menu;
    menu.InitFonts();
  
    // for some reason i have to explictly show the menu
    menu.Show();
  
    Camera2D camera;
    camera.zoom = 1.0f;
    camera.offset = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    camera.target = {0.0f, 0.0f};
    camera.rotation = 0.0f;

    Loop(menu, camera);
}

void Game::SetCurrentLevel(shared_ptr<Level> level) {
  m_CurrentLevel = level;
}

shared_ptr<Level> Game::GetCurrentLevel() {
  return Game::Get()->m_CurrentLevel;
}

void Game::StartGameplay() {
    if (m_isGamePlayRunning) {
        return;
    }

    m_isGamePlayRunning = true;

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
