#include "game.hpp"
#include "core/random.hpp"
#include "entity/player.hpp"
#include "core/log.hpp"
#include "menu/menu.hpp"
#include "gui/raspGui.hpp"
#include "registry.hpp"

#include "raylib.h"
#include "rlgl.h"

static s_Game* s_Instance;

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
    //Button button = Button(Vec2d(0.25f, 0.25f), Vec2d(0.5f, 0.5f), "funny", true, []() {Debug("hi");});
	
	double currentTime = 0.0;
	double lastTime = GetTime();
	double updateDrawTime = 0.0;
	double waitTime = 0.0;

	m_Camera.target = { 0.0f, 0.0f };
	m_Camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	m_Camera.rotation = 0.0f;
	m_Camera.zoom = 1.0f;

	m_EditorCamera.target = { 0.0f, 0.0f };
	m_EditorCamera.offset = { 0.0f, 0.0f };
	m_EditorCamera.rotation = 0.0f;
	m_EditorCamera.zoom = 1.0f;

	Registry.AddTile("Dirt", LoadTexture("Assets/dirt.png"));
	Registry.AddTile("Stone", LoadTexture("Assets/stone.png"));
	Registry.AddTile("Brick", LoadTexture("Assets/brick.png"));

    while (!WindowShouldClose()) {
		PollInputEvents();

		while (GetTime() - m_LastTime > 0.05f) {
			m_LastTime += GetTime() - m_LastTime;

			Tick();
		}

        if (m_GameRunning) {
			m_Camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

			m_CurrentLevel.OnUpdate();

            m_Camera.target = {m_CurrentLevel.GetPlayer().GetX() * 64.0f - 32.0f,
                            m_CurrentLevel.GetPlayer().GetY() * 64.0f - 32.0f};
        }

		if (m_EditorRunning) {
			m_Editor->OnUpdate();
		}

        BeginDrawing();

        ClearBackground(WHITE);
    
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
    
		// neither the game nor editor are running
		if (!m_GameRunning && !m_EditorRunning) {
			if (RaspGui::Button({0.05f, 0.65f, 0.2f, 0.1f}, "Play")) {
				StartGameplay();
			}

			if (RaspGui::Button({0.05f, 0.8f, 0.2f, 0.1f}, "Editor")) {
				StartEditor();
			}

			RaspGui::Label({0.1f, 0.05f, 0.8f, 0.2f}, "Welcome to raspberry!");
		}

		DrawText(TextFormat("FPS: %.1f", m_CurrentFPS), 10, 10, 20, BLACK);
    
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
	if (m_GameRunning) {
		m_CurrentLevel.Tick();
	}

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

    constexpr static int WINDOW_WIDTH = 21 * 64;
    constexpr static int WINDOW_HEIGHT = 12 * 64;
  
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raspberry");
  
    m_LastTime = GetTime();
  
    Random::Init();

    SetExitKey(KEY_F9);

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

    m_CurrentLevel.LoadLevelFromFile("hi.lvl");

    m_CurrentLevel.AddCollectable();
    m_CurrentLevel.GetCollectable().InitTextures();
}
