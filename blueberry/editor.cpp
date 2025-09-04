#include "editor.hpp"
#include "log.hpp"

#include "imgui.h"
#include "raylib.h"

void Editor::OnInit() {
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    Log(Log_Info, "Called On-Init %p!", GetWindowHandle());
}

static bool thing = false;

void Editor::OnUIRender(f32 ts) {
}
