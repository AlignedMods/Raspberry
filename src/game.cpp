#include "game.hpp"
#include "core/random.hpp"
#include "entity/player.hpp"
#include "core/log.hpp"
#include "menu/menu.hpp"
#include "gui/raspGui.hpp"

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
		Log(LogLevel::Critical, "There cannot be multiple game classes at once!!");
  }

  SetCurrentLevel(Level());

  SetTraceLogLevel(LOG_WARNING);
}

s_Game::~s_Game() {}

void s_Game::Loop(Camera2D& camera) {
    //Button button = Button(Vec2d(0.25f, 0.25f), Vec2d(0.5f, 0.5f), "funny", true, []() {Debug("hi");});
	
	double currentTime = 0.0;
	double lastTime = GetTime();
	double updateDrawTime = 0.0;
	double waitTime = 0.0;

    while (!WindowShouldClose()) {
		PollInputEvents();

		camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

        if (m_isGamePlayRunning) {
			m_CurrentLevel.OnUpdate();

            while (GetTime() - m_LastTime > 0.05f) {
                m_LastTime += GetTime() - m_LastTime;

                Tick();
            }
        
            camera.target = {m_CurrentLevel.GetPlayer().GetX(),
                            m_CurrentLevel.GetPlayer().GetY()};
        }

        BeginDrawing();

        ClearBackground(WHITE);
    
        if (m_isGamePlayRunning) {
            BeginMode2D(camera);

			m_CurrentLevel.OnRender();
      
            if (m_CurrentLevel.IsCollectableFound()) {
                DrawText("Found raspberry", 400, 600, 20, GREEN);
            }
      
            EndMode2D();
        }
    
        //menu.Draw();

		if (!m_isGamePlayRunning) {
			if (RaspGui::Button({0.3f, 0.35f, 0.4f, 0.3f}, "Play")) {
				StartGameplay();
			}

			RaspGui::Label({0.1f, 0.05f, 0.8f, 0.2f}, "Welcome to raspberry!");
		}
    
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
	m_CurrentLevel.Tick();
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
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raspberry");
  
    m_LastTime = GetTime();
  
    Random::Init();
  
    //SetTargetFPS(60);

    //ToggleBorderlessWindowed();
    SetExitKey(KEY_F9);
  
    //Menu menu;
    //menu.InitFonts();
  
    // for some reason i have to explictly show the menu
    //menu.Show();
  
    Camera2D camera;
    camera.zoom = 1.0f;
    camera.offset = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    camera.target = {0.0f, 0.0f};
    camera.rotation = 0.0f;

    Loop(camera);
}

void s_Game::SetCurrentLevel(const Level& level) {
	m_CurrentLevel = level;
}

Level& s_Game::GetCurrentLevel() {
	return m_CurrentLevel;
}

void s_Game::StartGameplay() {
    if (m_isGamePlayRunning) {
        return;
    }

    m_isGamePlayRunning = true;

    m_CurrentLevel.LoadLevelFromFile("hi.lvl");

    m_CurrentLevel.AddCollectable();
    m_CurrentLevel.GetCollectable().InitTextures();
}
