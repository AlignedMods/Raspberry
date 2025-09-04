#pragma once

#include "event/event.hpp"
#include "layerstack.hpp"
#include "log.hpp"
#include "registry/registry.hpp"
#include "types.hpp"

#define GLFW_INCLUDE_NONE
#include "external/glfw/include/GLFW/glfw3.h"

struct ApplicationSpecification {
    const char* name;
    u32 min_width, min_height;
    bool enable_audio;
    u32 FPS;
};

class CommandLineArgs {
public:
    inline CommandLineArgs(u32 argc, char** argv) {
        this->argc = argc;
        this->argv = argv;
    }

    inline const char* at(const u32 i) {
        if (i >= argc) {
            Log(Log_Critical, "Trying to access position in command line arguments that is out of bounds! {Position %lu, Size %lu}", i, argc);
            exit(1);
        }

        return argv[i];
    }

    inline u32 size() const {
        return argc;
    }

private:
    u32 argc;
    char** argv;
};

class Application {
public:
    Application(const ApplicationSpecification& spec);
    ~Application();

    void Run();

    void SetIcon(const Texture& texture);
    void SetIcon(const Image& image);

    bool IsInitialized();

    void SetTargetFPS(u32 fps);

    void Close();

    static Application& Get();
    Registry& GetRegistry() { return m_Registry; }
    LayerStack& GetLayerStack() { return m_Stack; }
    Dispatcher& GetDispatcher() { return m_Dispatcher; }

    // To be implemented by client!
    static Application* CreateApplication(const CommandLineArgs& args);

private:
    void OnUpdate();
    void OnRender();
    void OnUIRender();
    void OnEvent(const Event& event);

private:
    ApplicationSpecification m_Specification;

    u32 m_TargetFPS;

    bool m_Running;
    bool m_Initalized = false;

    f64 m_CurrentTime;
    f64 m_LastTime;
    f32 m_dt;

    f64 m_FixedUpdateTime;

    LayerStack m_Stack;
    Registry m_Registry;
    Dispatcher m_Dispatcher;

    friend class Dispatcher;
};
