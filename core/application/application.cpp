#include "application.hpp"
#include "rlImGui.h"

static Application* s_Instance = nullptr;

Application::Application(const ApplicationSpecification& spec) : m_Specification(spec) {
    InitWindow(1280, 720, spec.name);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(spec.min_width, spec.min_height);

    if (spec.enable_audio) {
        InitAudioDevice();
    }

    m_TargetFPS = spec.FPS;
    m_LastTime = GetTime();

    rlImGuiSetup(true);

    s_Instance = this;
}

Application::~Application() {
    if (m_Specification.enable_audio) {
        CloseAudioDevice();
    }

    rlImGuiShutdown();
    CloseWindow();
}

void Application::Run() {
    m_Running = true;

    while (m_Running) {
        m_Running = m_Running && !WindowShouldClose();

        PollInputEvents();
        OnUpdate();

        BeginDrawing();

        OnRender();
        OnUIRender();

        EndDrawing();

        SwapScreenBuffer();

        // Calculating the deltatime and timing

        m_CurrentTime = GetTime();

        f64 drawRenderTime = m_CurrentTime - m_LastTime;

        if (m_TargetFPS > 0) {
            f64 waitTime = (1.0 / static_cast<f64>(m_TargetFPS)) - drawRenderTime;
            WaitTime(waitTime);
            m_CurrentTime = GetTime();
            m_dt = static_cast<f32>(m_CurrentTime - m_LastTime);
        } else {
            m_dt = static_cast<f32>(drawRenderTime);
        }

        m_LastTime = m_CurrentTime;
    }
}

LayerStack& Application::GetLayerStack() {
    return m_Stack;
}

void Application::SetIcon(const Texture& texture) {
    SetIcon(LoadImageFromTexture(texture));
}

void Application::SetIcon(const Image& image) {
    SetWindowIcon(image);
}

void Application::SetTargetFPS(u32 fps) {
    m_TargetFPS = fps;
}

void Application::Close() {
    m_Running = false;
}

void Application::OnUpdate() {
    if (GetTime() - m_FixedUpdateTime > 0.0167) {
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
    ClearBackground({0x19, 0x19, 0x19, 0xff});

    for (auto layer : m_Stack.GetAllLayers()) {
        layer->OnRender();
    }
}

void Application::OnUIRender() {
    rlImGuiBeginDelta(m_dt);

    for (auto layer : m_Stack.GetAllLayers()) {
        layer->OnUIRender(m_dt);
    }

    rlImGuiEnd();
}

Application& Application::Get() {
    return *s_Instance;
}