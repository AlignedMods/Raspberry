// Blueberry - a single file Raspberry editor built using imgui and raylib

#include "types.hpp"

#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include "rlImGui.h"

#include <cmath>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

s_Registry Registry;

enum class States {
    Pan,
    Place,
    Edit,
    Destroy
};

enum class MouseStates {
    Click,
    Drag
};

enum class ApplicationStates {
    Splash,
    Editor
};

struct State {
    u32 targetFPS = 144;

    f64 currentTime = 0.0;
    f64 lastTime = 0.0;
    f64 updateDrawTime = 0.0;
    f64 waitTime = 0.0;

    f32 deltaTime = 0.0f;

    bool shouldClose = false;

    bool openFileDialog = false;
    ApplicationStates current = ApplicationStates::Splash;
};

static State state;
static Camera2D cam{};
static States currentState = States::Pan;
static MouseStates currentMouseState = MouseStates::Click;
static std::unordered_map<i32, f32> zooms = {
    {}};

static std::vector<EditorTile> tiles;

class Window {
  public:
    virtual void OnInit() = 0;

    virtual void OnUIRender() = 0;

  protected:
    bool m_Visible = true;
};

class OpenArchive : public Window {
  public:
    virtual void OnInit() override {
    }

    virtual void OnUIRender() override {
        if (m_Visible) {
            ImGui::Begin("Open Archive", &m_Visible);

            if (m_CurrentDirectory[0] == '\0') {
                strcpy(m_CurrentDirectory, GetWorkingDirectory());
            }

            ImGui::InputText("Current directory", m_CurrentDirectory, 256);

            if (ImGui::ArrowButton("back", ImGuiDir_Up)) {
                strcpy(m_CurrentDirectory, GetPrevDirectoryPath(m_CurrentDirectory));
            }

            ImGui::NewLine();

            // show file list in current directory
            if (DirectoryExists(m_CurrentDirectory)) {
                auto files = LoadDirectoryFiles(m_CurrentDirectory);

                for (int i = 0; i < files.count; i++) {

                    if (DirectoryExists(files.paths[i])) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    }

                    if (ImGui::Button(GetFileName(files.paths[i]))) {
                        if (DirectoryExists(files.paths[i])) {
                            strcpy(m_CurrentDirectory, files.paths[i]);
                        } else if (FileExists(files.paths[i])) {
                            Registry.RegisterArchive(files.paths[i]);
                            state.openFileDialog = false;
                        }
                    }

                    if (DirectoryExists(files.paths[i])) {
                        ImGui::PopStyleColor();
                    }
                }
            }

            ImGui::End();
        }
    }

  private:
    bool done = false;

    std::vector<const char *> m_Files;
    char m_CurrentDirectory[256]{};
};

static OpenArchive open;

static void SetupImGui();

static void OnUpdate(f32 deltaTime);
static void OnUIRender(f32 deltaTime);
static void OnRender();

static void PlaceTile(const Vector2 &pos);
static void DestroyTile(const Vector2 &pos);
static void KILLEMALL(); // kills well, ALL of em' (tiles)

static Vector2 GetPositionInWorld(const Vector2 &vec);

static void InitializeEditor();

int main() {
    InitWindow(1280, 720, "Blueberry - Raspberry editor");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);

    SetWindowMinSize(1280, 720);

    rlImGuiSetup(true);

    SetupImGui();

    while (!state.shouldClose) {
        if (WindowShouldClose()) {
            state.shouldClose = true;
            break;
        }

        PollInputEvents();

        OnUpdate(state.deltaTime);

        BeginDrawing();
        ClearBackground(WHITE);

        OnRender();
        OnUIRender(state.deltaTime);

        EndDrawing();
        SwapScreenBuffer();

        state.currentTime = GetTime();

        state.updateDrawTime = state.currentTime - state.lastTime;

        if (state.targetFPS > 0) {
            state.waitTime = (1.0 / static_cast<f64>(state.targetFPS)) - state.updateDrawTime;
            WaitTime(state.waitTime);
            state.currentTime = GetTime();
            state.deltaTime = static_cast<f32>(state.currentTime - state.lastTime);
        } else {
            state.deltaTime = static_cast<f32>(state.updateDrawTime);
        }

        state.lastTime = state.currentTime;
    }

    printf("Tiles placed: %lu", tiles.size());

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}

void SetupImGui() {
    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    colors[ImGuiCol_Text] = ImVec4(0.92f, 0.93f, 0.94f, 1.00f);                  // Light grey text for readability
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.52f, 0.54f, 1.00f);          // Subtle grey for disabled text
    colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);              // Dark background with a hint of blue
    colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);               // Slightly lighter for child elements
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);               // Popup background
    colors[ImGuiCol_Border] = ImVec4(0.28f, 0.29f, 0.30f, 0.60f);                // Soft border color
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);          // No border shadow
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);               // Frame background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);        // Frame hover effect
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);         // Active frame background
    colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);               // Title background
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);         // Active title background
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);      // Collapsed title background
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);             // Menu bar background
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);           // Scrollbar background
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);         // Dark accent for scrollbar grab
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.30f, 0.32f, 1.00f);  // Scrollbar grab hover
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.32f, 0.34f, 0.36f, 1.00f);   // Scrollbar grab active
    colors[ImGuiCol_CheckMark] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);             // Dark blue checkmark
    colors[ImGuiCol_SliderGrab] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);            // Dark blue slider grab
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);      // Active slider grab
    colors[ImGuiCol_Button] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);                // Dark blue button
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);         // Button hover effect
    colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.42f, 0.52f, 1.00f);          // Active button
    colors[ImGuiCol_Header] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);                // Header color similar to button
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);         // Header hover effect
    colors[ImGuiCol_HeaderActive] = ImVec4(0.32f, 0.42f, 0.52f, 1.00f);          // Active header
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);             // Separator color
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);      // Hover effect for separator
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);       // Active separator
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);            // Resize grip
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);     // Hover effect for resize grip
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.54f, 0.64f, 1.00f);      // Active resize grip
    colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);                   // Inactive tab
    colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);            // Hover effect for tab
    colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);             // Active tab color
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);          // Unfocused tab
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);    // Active but unfocused tab
    colors[ImGuiCol_PlotLines] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);             // Plot lines
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);      // Hover effect for plot lines
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);         // Histogram color
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);  // Hover effect for histogram
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);         // Table header background
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);     // Strong border for tables
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.24f, 0.25f, 0.26f, 1.00f);      // Light border for tables
    colors[ImGuiCol_TableRowBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);            // Table row background
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);         // Alternate row background
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.34f, 0.44f, 0.35f);        // Selected text background
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.46f, 0.56f, 0.66f, 0.90f);        // Drag and drop target
    colors[ImGuiCol_NavHighlight] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);          // Navigation highlight
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f); // Windowing highlight
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);     // Dim background for windowing
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);      // Dim background for modal windows

    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 11;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 4;
}

void OnUpdate(f32 deltaTime) {
    if (state.current == ApplicationStates::Editor) {
        int key = GetKeyPressed();

        switch (key) {
        case KEY_ONE:
            currentState = States::Pan;
            break;
        case KEY_TWO:
            currentState = States::Place;
            break;
        case KEY_THREE:
            currentState = States::Edit;
            break;
        case KEY_FOUR:
            currentState = States::Destroy;
            break;
        }

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            currentMouseState = MouseStates::Drag;
        } else {
            currentMouseState = MouseStates::Click;
        }

        if (currentState == States::Place && !ImGui::GetIO().WantCaptureMouse) {
            if (currentMouseState == MouseStates::Click && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaceTile(GetMousePosition());
            }

            if (currentMouseState == MouseStates::Drag && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                PlaceTile(GetMousePosition());
            }
        }

        if (GetMouseWheelMove() > 0.1f) {
            cam.zoom += 0.2f;
        }
        if (GetMouseWheelMove() < -0.1f) {
            cam.zoom -= 0.2f;
        }

        if (currentState == States::Pan && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            cam.target = Vector2Add(cam.target, Vector2Negate(GetMouseDelta()));
        }
    }
}

Texture romania{};
bool showRomania = false;

void OnRender() {
    if (romania.id == 0) {
        romania = LoadTexture("../Raspberry/Assets/textures/romania.png");
    }

    if (state.current == ApplicationStates::Editor) {
        BeginMode2D(cam);

        for (auto &tile : tiles) {
            tile.OnRender();
        }

        EndMode2D();
    }

    if (state.current == ApplicationStates::Splash) {
        DrawText("Welcome to Blueberry!", 200, 200, 40, BLACK);
    }

    // show romania flag if you turned on Toggle romania mode
    if (showRomania) {
        DrawTexturePro(romania, {0.0f, 0.0f, (f32)romania.width, (f32)romania.height}, {0.0f, 0.0f, (f32)GetScreenWidth(), (f32)GetScreenHeight()}, {0.0f, 0.0f}, 0.0f, WHITE);
        DrawText("Welcome to Romania!", 200, 200, 40, WHITE);
    }
}

void OnUIRender(f32 deltaTime) {
    rlImGuiBeginDelta(deltaTime);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // a LOT of flags
    // this is basiclly the top bar, which has things like the "File" menu and stuff
    ImGui::Begin("Blueberry", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking);
    ImGui::PopStyleVar();

    ImGui::SetWindowSize(ImVec2(GetScreenWidth(), GetScreenHeight()));
    ImGui::SetWindowPos(ImVec2(0, 0));

    ImGuiID id = ImGui::GetID("dockspace");
    ImGui::DockSpace(id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            if (ImGui::BeginMenu("New")) {
                if (ImGui::MenuItem("Map")) {
                    InitializeEditor();
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Open", "CTRL+Q")) {
                state.openFileDialog = true;
            }

            if (ImGui::MenuItem("Save", "CTRL+S")) {
                printf("save");
            }
            if (ImGui::MenuItem("Close", "ALT+F4")) {
                state.shouldClose = true;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            if (ImGui::MenuItem("Toggle Romania mode")) {
                showRomania = !showRomania;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (state.openFileDialog) {
        open.OnUIRender();
    }

    ImGui::End();

    rlImGuiEnd();

    // alt+f4 check in case if the user isn't using alt+f4 to close windoes
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_F4)) {
        state.shouldClose = true;
    }
}

void PlaceTile(const Vector2 &vec) {
    Vector2 pos = GetPositionInWorld(vec);
    tiles.push_back(EditorTile(GetPositionInWorld(vec), "hello"));
}

Vector2 GetPositionInWorld(const Vector2 &vec) {
    f32 x = vec.x;
    f32 y = vec.y;

    // we COULD use this but i am a fan of my own formula
    // return Vector2Divide(GetScreenToWorld2D(vec, cam), {32.0f, 32.0f});

    return {static_cast<f32>(std::floor(((x + (cam.target.x * cam.zoom - cam.offset.x)) / (cam.zoom * 32.0f)))),
            static_cast<f32>(std::floor(((y + (cam.target.y * cam.zoom - cam.offset.y)) / (cam.zoom * 32.0f))))};
}

std::vector<int> ints;

void InitializeEditor() {
    state.current = ApplicationStates::Editor;

    cam.zoom = 1.0f;
    cam.target = {0.0f, 0.0f};
    cam.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    cam.rotation = 0.0f;
}
