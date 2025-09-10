#include "application/entrypoint.hpp"
#include "application/application.hpp"

#include "editor.hpp"

Application* Application::CreateApplication(const CommandLineArgs& args) {
    ApplicationSpecification spec = {
        .name = "Blueberry",
        .width = 1280,
        .height = 720,
        .min_width = 640,
        .min_height = 360,
        .enable_audio = true,
        .FPS = 0,
    };

    Application* app = new Application(spec);
    // app->GetLayerStack().PushLayer<Editor>();

    return app;
}