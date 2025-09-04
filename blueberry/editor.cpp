#include "editor.hpp"
#include "log.hpp"

#include "external/glfw/include/GLFW/glfw3.h"
#include "imgui.h"
#include "raylib.h"

void Editor::OnInit() {
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    Log(Log_Info, "Called On-Init %p!", GetWindowHandle());
}

static bool thing = false;

void Editor::OnUIRender(f32 ts) {
    ImGui::Begin("Test window!");

    if (ImGui::Button("hi button!")) {
        thing = true;
    }

    if (thing) {
        ImGui::Begin("hi", &thing);

        ImGui::End();
    }

    ImGui::End();

    ImGui::ShowDemoWindow();
}
