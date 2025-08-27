#pragma once

#include "layer.hpp"
#include "layerstack.hpp"
#include "log.hpp"
#include "types.hpp"

struct ApplicationSpecification {
    const char* name;
    u32 min_width, min_height;
    bool enable_audio;
    u32 FPS;
};

class CommandLineArgs {
  public:
    inline CommandLineArgs(u32 argc, char** argv) {
        this->argc;
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

    LayerStack& GetLayerStack();

    void SetIcon(const Texture& texture);
    void SetIcon(const Image& image);

    void SetTargetFPS(u32 fps);

    void Close();

    static Application& Get();

    // To be implemented by client!
    static Application* CreateApplication(const CommandLineArgs& args);

  private:
    void OnUpdate();
    void OnRender();
    void OnUIRender();

  private:
    ApplicationSpecification m_Specification;
    LayerStack m_Stack;

    u32 m_TargetFPS;

    bool m_Running;

    f64 m_CurrentTime;
    f64 m_LastTime;
    f32 m_dt;

    f64 m_FixedUpdateTime;
};