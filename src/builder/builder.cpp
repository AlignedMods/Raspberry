#include "builder.hpp"
#include "builder/editor/editor.hpp"
#include "core/core.hpp"
#include "renderer/renderer.hpp"
#include "raylib.h"
#include "utils/types.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION

//#include "core/tinyfiledialogs.h"

// Dialog type
typedef enum DialogType {
    DIALOG_OPEN_FILE = 0,
    DIALOG_OPEN_FILE_MULTI,
    DIALOG_OPEN_DIRECTORY,
    DIALOG_SAVE_FILE,
    DIALOG_MESSAGE,
    DIALOG_TEXTINPUT,
    DIALOG_OTHER
} DialogType;

// Multiplatform file dialogs
//int GuiFileDialog(int dialogType, const char *title, char *fileName, const char *filters, const char *message)
//{
//    int result = -1;
//
//#if defined(CUSTOM_MODAL_DIALOGS)
//    switch (dialogType)
//    {
//        case DIALOG_OPEN_FILE: /* TODO: Load file modal dialog */ break;
//        case DIALOG_OPEN_FILE_MULTI: /* TODO: Load multiple files modal dialog */ break;
//        case DIALOG_OPEN_DIRECTORY: /* TODO: Load directory modal dialog */ break;
//        case DIALOG_SAVE_FILE: /* TODO: Save file modal dialog */ break;
//        case DIALOG_MESSAGE: result = GuiMessageBox((Rectangle){ GetScreenWidth()/2 - 160, GetScreenHeight()/2 - 120, 320, 120 }, title, message, filters); break;
//        case DIALOG_TEXTINPUT: result = GuiTextInputBox((Rectangle){ GetScreenWidth()/2 - 160, GetScreenHeight()/2 - 120, 320, 120 }, title, message, filters, fileName, 512, NULL); break;
//        default: break;
//    }
//#else   // Use native OS dialogs (tinyfiledialogs)
//
//    char *tempFileName = NULL;
//    int filterCount = 0;
//    const char **filterSplit = (const char **)TextSplit(filters, ';', &filterCount);
//    
//    switch (dialogType)
//    {
//        case DIALOG_OPEN_FILE: tempFileName = tinyfd_openFileDialog(title, fileName, filterCount, filterSplit, message, 0); break;
//        case DIALOG_OPEN_FILE_MULTI: tempFileName = tinyfd_openFileDialog(title, fileName, filterCount, filterSplit, message, 1); break;
//        case DIALOG_OPEN_DIRECTORY: tempFileName = tinyfd_selectFolderDialog(title, fileName); break;
//        case DIALOG_SAVE_FILE: tempFileName = tinyfd_saveFileDialog(title, fileName, filterCount, filterSplit, message); break;
//        case DIALOG_MESSAGE: result = tinyfd_messageBox(title, message, "ok", "info", 0); break;
//        case DIALOG_TEXTINPUT: tempFileName = tinyfd_inputBox(title, message, ""); break;
//        default: break;
//    }
//
//    if (tempFileName != NULL) 
//    {
//        strcpy(fileName, tempFileName);
//        result = 1;
//    }
//    else result = 0;
//#endif
//
//    return result;
//}

static Builder* s_Instance = nullptr;
static Renderer* s_Renderer = nullptr;

Builder::Builder() {
	if (!s_Instance) {
		s_Renderer = new Renderer();
		s_Instance = this;
	} else {
		Error("Builder class already exists!\nThere cannot be multiple builder classes at the same time!", 1);

        SetTraceLogLevel(LOG_WARNING);
	}
}

Builder::~Builder() {
	delete s_Renderer;
	delete m_Editor;
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

     // Custom file dialog

    char* tempFileName = NULL;
	
	SetTargetFPS(60);

    bool fullscreen = false;
	
	// Load the lavanda style
    GuiLoadStyle("Assets/GuiStyles/style_lavanda.rgs");
	// We want to center the text in any container
	GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_MIDDLE);

    //GuiSetStyle(DEFAULT, TEXT_SIZE, 70);

    Font font = LoadFont("Assets/Fonts/JandaManateeSolid.ttf");

    GuiSetFont(font);

	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.rotation = 0.0f;
	camera.target = {0.0f, 0.0f};

	while (!WindowShouldClose()) {
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

		if (!m_Editor) {
			s_Renderer->RenderText("Welcome to Raspberry Builder!", 0, 100);
			s_Renderer->RenderText("To get started press the Create New Level button!", 0, 150);

			// Set the text size to 30 for buttons
			GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

			if (s_Renderer->RenderButton(Vec2d(0.25f, 0.25f), Vec2d(0.5, 0.5), "Create New Level")) {
				std::cout << "Clicked!\n";
				m_Editor = new Editor(camera);
			}

            
            if (s_Renderer->RenderButton(Vec2d(0.25f, 0.25f), Vec2d(0.5, 0.5), "Load Existing Level")) {
                //tempFileName = tinyfd_selectFolderDialog("Skibidfi", GetWorkingDirectory());
            }

		} else {
			BeginMode2D(camera);
			m_Editor->Draw();
			EndMode2D();

			m_Editor->DrawMenu();
		}

		EndDrawing();
	}

	CloseWindow();
}

Builder* Builder::Get() {
	return s_Instance;
}

Renderer* Builder::GetRenderer() {
	return s_Renderer;
}

