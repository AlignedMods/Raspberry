#include "application.hpp"
#include "event/event.hpp"
#include "event/key_events.hpp"
#include "event/window_events.hpp"
#include "event/mouse_events.hpp"
#include "input/keycodes.hpp"

#include "external/glfw/include/GLFW/glfw3.h"
#include "imgui_impl_raylib.h"
#include "rlgl.h"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

KeyCode GLFWToRaspberry(const i32 key) {
    switch (key) {
        // non-printable keys
        case GLFW_KEY_ESCAPE: return KeyCode::Escape;
        case GLFW_KEY_TAB: return KeyCode::Tab;
        case GLFW_KEY_CAPS_LOCK: return KeyCode::CapsLock;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT: return KeyCode::Shift;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL: return KeyCode::Ctrl;
        case GLFW_KEY_LEFT_SUPER:
        case GLFW_KEY_RIGHT_SUPER: return KeyCode::Win;
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT: return KeyCode::Alt;
        case GLFW_KEY_SPACE: return KeyCode::Space;
        case GLFW_KEY_LEFT: return KeyCode::Left;
        case GLFW_KEY_DOWN: return KeyCode::Down;
        case GLFW_KEY_UP: return KeyCode::Up;
        case GLFW_KEY_RIGHT: return KeyCode::Right;
        case GLFW_KEY_PAGE_DOWN: return KeyCode::PageDown;
        case GLFW_KEY_PAGE_UP: return KeyCode::PageUp;
        case GLFW_KEY_DELETE: return KeyCode::Del;
        case GLFW_KEY_HOME: return KeyCode::Home;
        case GLFW_KEY_BACKSPACE: return KeyCode::Backspace;
        case GLFW_KEY_ENTER: return KeyCode::Enter;

        // printable keys
        case GLFW_KEY_0: return KeyCode::Num0;
        case GLFW_KEY_1: return KeyCode::Num1;
        case GLFW_KEY_2: return KeyCode::Num2;
        case GLFW_KEY_3: return KeyCode::Num3;
        case GLFW_KEY_4: return KeyCode::Num4;
        case GLFW_KEY_5: return KeyCode::Num5;
        case GLFW_KEY_6: return KeyCode::Num6;
        case GLFW_KEY_7: return KeyCode::Num7;
        case GLFW_KEY_8: return KeyCode::Num8;
        case GLFW_KEY_9: return KeyCode::Num9;
        case GLFW_KEY_A: return KeyCode::A;
        case GLFW_KEY_B: return KeyCode::B;
        case GLFW_KEY_C: return KeyCode::C;
        case GLFW_KEY_D: return KeyCode::D;
        case GLFW_KEY_E: return KeyCode::E;
        case GLFW_KEY_F: return KeyCode::F;
        case GLFW_KEY_G: return KeyCode::G;
        case GLFW_KEY_H: return KeyCode::H;
        case GLFW_KEY_I: return KeyCode::I;
        case GLFW_KEY_J: return KeyCode::J;
        case GLFW_KEY_K: return KeyCode::K;
        case GLFW_KEY_L: return KeyCode::L;
        case GLFW_KEY_M: return KeyCode::M;
        case GLFW_KEY_N: return KeyCode::N;
        case GLFW_KEY_O: return KeyCode::O;
        case GLFW_KEY_P: return KeyCode::P;
        case GLFW_KEY_Q: return KeyCode::Q;
        case GLFW_KEY_R: return KeyCode::R;
        case GLFW_KEY_S: return KeyCode::S;
        case GLFW_KEY_T: return KeyCode::T;
        case GLFW_KEY_U: return KeyCode::U;
        case GLFW_KEY_V: return KeyCode::V;
        case GLFW_KEY_W: return KeyCode::W;
        case GLFW_KEY_X: return KeyCode::X;
        case GLFW_KEY_Y: return KeyCode::Y;
        case GLFW_KEY_Z: return KeyCode::Z;

        // function keys
        case GLFW_KEY_F1: return KeyCode::F1;
        case GLFW_KEY_F2: return KeyCode::F2;
        case GLFW_KEY_F3: return KeyCode::F3;
        case GLFW_KEY_F4: return KeyCode::F4;
        case GLFW_KEY_F5: return KeyCode::F5;
        case GLFW_KEY_F6: return KeyCode::F6;
        case GLFW_KEY_F7: return KeyCode::F7;
        case GLFW_KEY_F8: return KeyCode::F8;
        case GLFW_KEY_F9: return KeyCode::F9;
    }

    // this shouldn't really happen but i need to shut up the compiler
    return KeyCode::None;
}

ImGuiKey RaspberryToImGui(const KeyCode key) {
    switch (key) {
        case KeyCode::Escape: return ImGuiKey_Escape;
        case KeyCode::Tab: return ImGuiKey_Tab;
        case KeyCode::Shift: return ImGuiMod_Shift;
        case KeyCode::Ctrl: return ImGuiMod_Ctrl;
        case KeyCode::Left: return ImGuiKey_LeftArrow;
        case KeyCode::Down: return ImGuiKey_DownArrow;
        case KeyCode::Up: return ImGuiKey_UpArrow;
        case KeyCode::Right: return ImGuiKey_RightArrow;
        case KeyCode::Backspace: return ImGuiKey_Backspace;
        case KeyCode::Enter: return ImGuiKey_Enter;
        case KeyCode::A: return ImGuiKey_A;
        case KeyCode::V: return ImGuiKey_V;
        default: return ImGuiKey_None;
    }

    return ImGuiKey_None;
}

static Application* s_Instance = nullptr;

static GLFWwindowsizefun RaylibResize;

static void CallbackKey(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    auto& dispatcher = Application::Get().GetDispatcher();

    const KeyCode keycode = GLFWToRaspberry(key);

    if (action == GLFW_PRESS) {
        dispatcher.Post(KeyPressedEvent(keycode, false));
    } else if (action == GLFW_RELEASE) {
        dispatcher.Post(KeyReleasedEvent(keycode));
    } else if (action == GLFW_REPEAT) {
        dispatcher.Post(KeyPressedEvent(keycode, true));
    }
}

static void CallbackChar(GLFWwindow* window, u32 codepoint) {
    auto& dispatcher = Application::Get().GetDispatcher();

    dispatcher.Post(KeyTypedEvent(codepoint));
}

static void CallbackWindowClose(GLFWwindow* window) {
    auto& dispatcher = Application::Get().GetDispatcher();

    dispatcher.Post(WindowCloseEvent());
}

static void CallbackWindowResize(GLFWwindow* window, i32 width, i32 height) {
    auto& dispatcher = Application::Get().GetDispatcher();

    RaylibResize(window, width, height);
    
    dispatcher.Post(WindowResizeEvent(width, height));
}

static void CallbackCursorPos(GLFWwindow* window, f64 x, f64 y) {
    auto& dispatcher = Application::Get().GetDispatcher();

    dispatcher.Post(MouseMovedEvent(x, y));
}

static void CallbackMouseButton(GLFWwindow* window, i32 button, i32 action, i32 mods) {
    auto& dispatcher = Application::Get().GetDispatcher();

    if (action == GLFW_PRESS) {
        dispatcher.Post(MouseButtonPressedEvent((MouseButton)button));
    } else if (action == GLFW_RELEASE) {
        dispatcher.Post(MouseButtonReleasedEvent((MouseButton)button));
    }
}

static void CallbackScroll(GLFWwindow* window, f64 x, f64 y) {
    auto& dispatcher = Application::Get().GetDispatcher();

    dispatcher.Post(MouseScrolledEvent(y));
}

Application::Application(const ApplicationSpecification& spec) : m_Specification(spec) {
    InitWindow(1280, 720, spec.name);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(spec.min_width, spec.min_height);

    GLFWwindow* window = static_cast<GLFWwindow*>(GetWindowHandle());

    // keyboard input
    glfwSetKeyCallback(window, CallbackKey);
    glfwSetCharCallback(window, CallbackChar);

    // window events
    glfwSetWindowCloseCallback(window, CallbackWindowClose);
    RaylibResize = glfwSetWindowSizeCallback(window, CallbackWindowResize);

    // mouse events
    glfwSetCursorPosCallback(window, CallbackCursorPos);
    glfwSetMouseButtonCallback(window, CallbackMouseButton);
    glfwSetScrollCallback(window, CallbackScroll);

    if (spec.enable_audio) {
        InitAudioDevice();
    }

    m_TargetFPS = spec.FPS;
    m_LastTime = GetTime();

    rlImGuiSetup(true);

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

void Application::SetIcon(const Texture& texture) {
    SetIcon(LoadImageFromTexture(texture));
}

void Application::SetIcon(const Image& image) {
    SetWindowIcon(image);
}

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

extern ImGuiContext* GlobalContext;

void Application::OnUIRender() {
    ImGuiIO& io = ImGui::GetIO();

    io.DeltaTime = m_dt;

    ImGui::SetCurrentContext(GlobalContext);
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    for (auto layer : m_Stack.GetAllLayers()) {
        layer->OnUIRender(m_dt);
    }

    ImGui::SetCurrentContext(GlobalContext);
    ImGui::Render();
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}

void Application::OnEvent(const Event& event) {
    // TODO: implement imgui input blackend!
    ImGuiIO io = ImGui::GetIO();

    const auto type = event.GetEventType();

    if (type == EventType::MouseMoved) {
        const auto& m = EVENT_CAST(MouseMovedEvent);

        if (!io.WantSetMousePos) {
            // Log(Log_Info, "hell yeah we want the mouse!");
            io.AddMousePosEvent((f32)m.GetX(), (f32)m.GetY());
        }
    }

    if (type == EventType::MouseButtonPressed) {
        const auto& m = EVENT_CAST(MouseButtonPressedEvent);

        ImGuiMouseButton btn;
        const MouseButton raybtn = m.GetButton();

        switch (raybtn) {
            case MOUSE_BUTTON_LEFT:
                btn = ImGuiMouseButton_Left;
                break;
            case MOUSE_BUTTON_RIGHT:
                btn = ImGuiMouseButton_Right;
                break;
            default:
                btn = ImGuiMouseButton_Left;
                break;
        };

        io.AddMouseButtonEvent(btn, true);
    }

    if (type == EventType::MouseButtonReleased) {
        const auto& m = EVENT_CAST(MouseButtonReleasedEvent);

        ImGuiMouseButton btn;
        const MouseButton raybtn = m.GetButton();

        switch (raybtn) {
            case MOUSE_BUTTON_LEFT:
                btn = ImGuiMouseButton_Left;
                break;
            case MOUSE_BUTTON_RIGHT:
                btn = ImGuiMouseButton_Right;
                break;
            default:
                btn = ImGuiMouseButton_Left;
                break;
        };

        io.AddMouseButtonEvent(btn, false);
    }

    if (event.GetEventType() == EventType::MouseScrolled) {
        const auto& m = EVENT_CAST(MouseScrolledEvent);

        io.AddMouseWheelEvent(0, m.GetScroll());
    }

    if (event.GetEventType() == EventType::KeyTyped) {
        const auto& k = EVENT_CAST(KeyTypedEvent);

        if (io.WantCaptureKeyboard) {
            Log(Log_Info, "Typing!");
            io.AddInputCharacter(k.GetKey());
        }
    }

    if (event.GetEventType() == EventType::KeyPressed) {
        const auto& k = EVENT_CAST(KeyPressedEvent);

        io.AddKeyEvent(RaspberryToImGui(k.GetKeyCode()), true);
    }

    if (event.GetEventType() == EventType::KeyReleased) {
        const auto& k = EVENT_CAST(KeyReleasedEvent);

        io.AddKeyEvent(RaspberryToImGui(k.GetKeyCode()), false);
    }

    for (Layer* layer : m_Stack.GetAllLayers()) {
        layer->OnEvent(event);
    }
}

Application& Application::Get() {
    return *s_Instance;
}
