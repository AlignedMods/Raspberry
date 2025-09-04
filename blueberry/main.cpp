#include "application/entrypoint.hpp"
#include "editor.hpp"

Application* Application::CreateApplication(const CommandLineArgs& args) {
    ApplicationSpecification spec = {
        .name = "Blueberry",
        .min_width = 640,
        .min_height = 360,
        .enable_audio = true,
        .FPS = 144,
    };

    Application* app = new Application(spec);
    app->GetLayerStack().PushLayer<Editor>();

    return app;
}
