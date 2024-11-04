#include "builder.hpp"
#include "builder/editor/editor.hpp"
#include "core/core.hpp"
#include "renderer/renderer.hpp"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static Builder* s_Instance = nullptr;
static Renderer* s_Renderer = nullptr;

Builder::Builder() {
	if (!s_Instance) {
		s_Renderer = new Renderer();
		s_Instance = this;
	} else {
		Error("Builder class already exists!\nThere cannot be multiple builder classes at the same time!", 1);
	}
}

Builder::~Builder() {
	delete s_Renderer;
	delete m_Editor;
}

void Builder::Run() {
	const int windowWidth = 21 * 64;
	const int windowHeight = 12 * 64;

	InitWindow(windowWidth, windowHeight, "Raspberry Builder");
	
	SetTargetFPS(60);
	
	// Set font color to black (0x000000ff is black)
	GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000ff);
	// We want to center the text in any container
	GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_MIDDLE);

	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.rotation = 0.0f;
	camera.target = {0.0f, 0.0f};

	while (!WindowShouldClose()) {
		if (m_Editor) {
			m_Editor->Update();
		}

		BeginDrawing();

		ClearBackground(WHITE);

		// Set the text size to be bigger than the normal size
		GuiSetStyle(DEFAULT, TEXT_SIZE, 40);

		if (!m_Editor) {
			s_Renderer->RenderText("Welcome to Raspberry Builder!", 0, 100);
			s_Renderer->RenderText("To get started press the Create New Level button!", 0, 150);

			// Set the text size to 30 for buttons
			GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

			if (s_Renderer->RenderButton(Rectangle{25, 600, 300, 50}, "Create New Level")) {
				std::cout << "Clicked!\n";
				m_Editor = new Editor(camera);
			}

			if (s_Renderer->RenderButton(Rectangle{25, 675, 300, 50}, "Load Existing Level")) {
				std::cout << "Clicked other button!\n";
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

