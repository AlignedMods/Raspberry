#include "editor.hpp"
#include "log.hpp"

#include "external/glfw/include/GLFW/glfw3.h"
#include "imgui.h"
#include "raylib.h"

void Editor::OnInit() {
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    Log(Log_Info, "Called On-Init %p!", GetWindowHandle());

    // buttons
    colors[ImGuiCol_Button] = ImVec4(0.2, 0.2, 0.2, 1);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3, 0.3, 0.3, 1);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.4, 0.4, 0.4, 1);

    // headers
    colors[ImGuiCol_Header] = ImVec4(0.2, 0.2, 0.2, 1);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25, 0.25, 0.25, 1);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.3, 0.3, 0.3, 1);

    // checkboxes
    colors[ImGuiCol_CheckMark] = ImVec4(0.9, 0.2, 0.2, 1);

    // frame backgrounds (checkbox, radio)
    colors[ImGuiCol_FrameBg] = ImVec4(0.6, 0.3, 0.3, 0.7);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.7, 0.3, 0.3, 0.8);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.8, 0.4, 0.4, 0.9);

    // sliders
    colors[ImGuiCol_SliderGrab] = ImVec4(0.7, 0.4, 0.4, 0.7);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7, 0.4, 0.4, 0.9);

    // docking
    colors[ImGuiCol_DockingPreview] = ImVec4(0.6, 0.3, 0.3, 0.7);

    // tabs
    colors[ImGuiCol_Tab] = ImVec4(0.3, 0.1, 0.1, 0.3);
    colors[ImGuiCol_TabHovered] = ImVec4(0.3, 0.1, 0.1, 0.5);
    colors[ImGuiCol_TabSelected] = ImVec4(0.3, 0.1, 0.1, 0.6);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.4, 0.1, 0.1, 1);
    colors[ImGuiCol_TabDimmed] = ImVec4(0.2, 0.1, 0.1, 0.2);
    colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.2, 0.1, 0.1, 0.4);

    // windows
    colors[ImGuiCol_WindowBg] = ImVec4(0.05, 0.05, 0.05, 1);
    colors[ImGuiCol_TitleBg] = ImVec4(0.5, 0.2, 0.2, 1);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.8, 0.2, 0.2, 1);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.4, 0.2, 0.2, 0.8);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5, 0.2, 0.2, 0.9);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.6, 0.2, 0.2, 1);

    // the resize thingy on the edge on windows!
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.8, 0.25, 0.25, 1);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7, 0.3, 0.3, 1);
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
