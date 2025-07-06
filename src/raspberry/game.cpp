#include "raspberry/game.hpp"
#include "shared/random.hpp"
#include "raspberry/entity/player.hpp"
#include "shared/log.hpp"
#include "raspberry/menu/menu.hpp"
#include "raspberry/gui/raspGui.hpp"

#include "raylib.h"

#include <memory>

using std::make_shared;
using std::shared_ptr;

static s_Game* s_Instance;

void Callback() {
    Debug("Clicked");
}

s_Game::s_Game() {
  if (!s_Instance) {
    s_Instance = this;
  } else {
    Error(
        "s_Game class already exists!\nThere cannot be multiple game classes "
        "at the same time!",
        1);
  }

  SetCurrentLevel(make_shared<Level>());

  SetTraceLogLevel(LOG_WARNING);
}

s_Game::~s_Game() {}

void s_Game::Loop(Menu& menu, Camera2D& camera) {
    //Button button = Button(Vec2d(0.25f, 0.25f), Vec2d(0.5f, 0.5f), "funny", true, []() {Debug("hi");});
	
	double currentTime = 0.0;
	double lastTime = GetTime();
	double updateDrawTime = 0.0;
	double waitTime = 0.0;

    while (!WindowShouldClose()) {
		PollInputEvents();

		camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

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

        //button.OnUpdate();
    
        menu.Update();
    
        BeginDrawing();

        //button.OnRender();
    
        ClearBackground(WHITE);
    
        if (m_isGamePlayRunning) {
            BeginMode2D(camera);

            for (auto tile : m_CurrentLevel->GetAllTiles()) {
                tile->OnRender();
            }
      
            //DrawRectangle(80, 90, 70, 200, BLACK);
      
            m_CurrentLevel->GetPlayer()->OnRender();
      
            if (m_CurrentLevel->IsCollectableFound()) {
                DrawText("Found raspberry", 400, 600, 20, GREEN);
            }
      
            m_CurrentLevel->GetCollectable().Draw();
      
            EndMode2D();
        }
    
        menu.Draw();
    
        EndDrawing();

		SwapScreenBuffer();

		currentTime = GetTime();

		updateDrawTime = currentTime - lastTime;

		if (targetFPS > 0) {
			waitTime = (1.0 / static_cast<double>(targetFPS)) - updateDrawTime;
			WaitTime(waitTime);
			currentTime = GetTime();
			deltaTime = static_cast<float>(currentTime - lastTime);
		} else {
			deltaTime = static_cast<float>(updateDrawTime);
		}

		lastTime = currentTime;
    }
}

// NOTE: a tick is basically a 20th of a second.
void s_Game::Tick() {
    m_CurrentLevel->GetPlayer()->UpdateTextures();
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

    constexpr static int WINDOW_WIDTH = 21 * 64;
    constexpr static int WINDOW_HEIGHT = 12 * 64;
  
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raspberry");
  
    m_LastTime = GetTime();
  
    Random::Init();
  
    //SetTargetFPS(60);

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

void s_Game::SetCurrentLevel(shared_ptr<Level> level) {
	m_CurrentLevel = level;
}

shared_ptr<Level> s_Game::GetCurrentLevel() {
	return m_CurrentLevel;
}

void s_Game::StartGameplay() {
    if (m_isGamePlayRunning) {
        return;
    }

    m_isGamePlayRunning = true;

    m_CurrentLevel->AddPlayer();

    m_CurrentLevel->AddCollectable();
    m_CurrentLevel->GetCollectable().InitTextures();

    m_CurrentLevel->LoadLevelFromFile("hi.lvl");
}
