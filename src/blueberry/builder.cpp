#include "builder.hpp"
#include "blueberry/editor/editor.hpp"
#include "renderer/renderer.hpp"
#include "shared/log.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
//
//#undef RAYGUI_IMPLEMENTATION

//#include "core/tinyfiledialogs.h"

static Builder* s_Instance = nullptr;

Builder::Builder() {
	if (!s_Instance) {
		s_Instance = this;
	} else {
		Error("Builder class already exists!\nThere cannot be multiple builder classes at the same time!", 1);

        SetTraceLogLevel(LOG_WARNING);
	}
}

Builder::~Builder() {
	delete m_Editor;
}

void Builder::LoadEditor() {
    m_Editor = new Editor(m_Camera);
}

void Builder::Run() {
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

	const int windowWidth = 21 * 64;
	const int windowHeight = 12 * 64;

    //SetConfigFlags(FLAG_FULLSCREEN_MODE);  
	InitWindow(windowWidth, windowHeight, "Raspberry Builder");

	//SetTargetFPS(60);

    bool fullscreen = false;

    //GuiSetStyle(DEFAULT, TEXT_SIZE, 70);

    //Font font = LoadFont("Assets/Fonts/JandaManateeSolid.ttf");
	//
	
	GuiLoadStyle("Assets/GuiStyles/style_dark.rgs");

	// We want to center the text in any container
	GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_MIDDLE);

	m_Camera.zoom = 1.0f;
	m_Camera.rotation = 0.0f;
	m_Camera.target = {0.0f, 0.0f};

	while (!WindowShouldClose()) {
		PollInputEvents();

		if (m_Editor) {
			m_Editor->Update();
		}

        if (IsKeyPressed(KEY_F11))
        {
            int monitor = GetCurrentMonitor();
            if (!IsWindowFullscreen())
            {
                // Resize to monitor resolution before going fullscreen
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
            } else {
                SetWindowSize(windowWidth, windowHeight);
            }
            ToggleFullscreen();
        }

		BeginDrawing();

		ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        //DrawTexture(texture, GetScreenWidth()/2 - texture.width/2, GetScreenHeight()/2 - texture.height/2 - 5, WHITE);
        //DrawRectangleLines(GetScreenWidth()/2 - texture.width/2, GetScreenHeight()/2 - texture.height/2 - 5, texture.width, texture.height, BLACK);

		// Set the text size to be bigger than the normal size

		GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

		if (!m_Editor) {
			Renderer.RenderText("Welcome to Blueberry!", 0, 100);
			Renderer.RenderText("To get started press the Create New Level button!", 0, 150);

			GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

			if (Renderer.RenderButton(Rectangle{25, 600, 300, 50}, "Create New Level")) {
				m_Editor = new Editor(m_Camera);
			}

			if (Renderer.RenderButton(Rectangle{25, 675, 300, 50}, "Load Existing Level")) {
			}
		} else {
			BeginMode2D(m_Camera);
			m_Editor->Draw();
			EndMode2D();

			m_Editor->DrawMenu();
		}

		EndDrawing();

		SwapScreenBuffer();
	}

	CloseWindow();
}

Builder* Builder::Get() {
	return s_Instance;
}
