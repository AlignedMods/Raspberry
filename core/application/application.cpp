#include "application.hpp"
#include "event/event.hpp"
#include "event/key_events.hpp"
#include "event/window_events.hpp"
#include "event/mouse_events.hpp"
#include "input/keycodes.hpp"
#include "input/mousecodes.hpp"
#include "platform/glfw_window.hpp"
#include "platform/opengl3_renderer.hpp"

#include "imgui.h"
#include "glad/glad.h"

static Application* s_Instance = nullptr;

Application::Application(const ApplicationSpecification& spec) 
    : m_Specification(spec) {
    WindowData data;
    data.name = spec.name;
    data.width = spec.width;
    data.height = spec.height;

    m_Window = new Window_GLFW(data); // also sets up opengl

    Vector2 viewport = {data.width, data.height};
    m_Renderer = new Renderer_OpenGL3(viewport);

    m_TargetFPS = spec.FPS;
    m_LastTime = m_Window->GetTime();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

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

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    m_Initalized = true;

    Log(Log_Info, "Constructed application!");

    for (auto& layer : m_Stack.GetAllLayers()) {
        layer->OnInit();
    }

    s_Instance = this;
}

Application::~Application() {
    delete m_Window;
}

void Application::Run() {
    m_Running = true;

    while (m_Running) {
        m_Running = m_Running && !m_Window->ShouldClose();

        m_Window->OnUpdate();
        OnUpdate();

        m_Window->OnRenderStart();

        OnRender();
        OnUIRender();

        m_Window->OnRenderFinish();

        // Calculating the deltatime and timing
        m_CurrentTime = m_Window->GetTime();

        f64 drawRenderTime = m_CurrentTime - m_LastTime;

        if (m_TargetFPS > 0) {
            f64 waitTime = (1.0 / static_cast<f64>(m_TargetFPS)) - drawRenderTime;
            m_Window->SleepSeconds(waitTime);
            m_CurrentTime = m_Window->GetTime();
            m_dt = static_cast<f32>(m_CurrentTime - m_LastTime);
        } else {
            m_dt = static_cast<f32>(drawRenderTime);
        }

        m_LastTime = m_CurrentTime;
    }
}

void Application::SetIcon(const Texture& texture) {}

bool Application::IsInitialized() {
    return m_Initalized;
}

void Application::SetTargetFPS(u32 fps) {
    m_TargetFPS = fps;
}

void Application::Close() {
    m_Running = false;
}

void Application::OnUpdate() {
    if (m_Window->GetTime() - m_FixedUpdateTime > 0.0167) {
        m_FixedUpdateTime += 0.0167;

        for (auto layer : m_Stack.GetAllLayers()) {
            layer->OnFixedUpdate();
        }
    }

    for (auto layer : m_Stack.GetAllLayers()) {
        layer->OnUpdate(m_dt);
    }
}

void Application::OnRender() {
    // ClearBackground({0x19, 0x19, 0x19, 0xff});

    m_Renderer->DrawTriangle({{100.0f, 500.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, {{300.0f, 100.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, {{500.0f, 500.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});

    for (auto layer : m_Stack.GetAllLayers()) {
        layer->OnRender();
    }
}

void Application::OnUIRender() {
    ImGuiIO& io = ImGui::GetIO();

    io.DeltaTime = m_dt;

    ImGui::Begin("Info");

    ImGui::Text("ImGui deltatime: %f", ImGui::GetIO().DeltaTime);
    ImGui::Text("Application deltatime: %f", m_dt);
    ImGui::Text("ImGui framerate: %f", ImGui::GetIO().Framerate);

    ImGui::End();

    // ImGui::NewFrame();

    for (auto layer : m_Stack.GetAllLayers()) {
        layer->OnUIRender(m_dt);
    }

    // ImGui::Render();
}

void Application::OnEvent(const Event& event) {
    // TODO: implement imgui input blackend!
    ImGuiIO io = ImGui::GetIO();

    const auto type = event.GetEventType();

    if (type == EventType::WindowResize) {
        const auto& wr = EVENT_CAST(WindowResizeEvent);
        m_Renderer->UpdateViewport({(f32)wr.GetWidth(), (f32)wr.GetHeight()});
    }
}

Application& Application::Get() {
    return *s_Instance;
}
