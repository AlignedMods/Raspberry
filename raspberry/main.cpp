#include "application/entrypoint.hpp"
#include "game.hpp"

Application* Application::CreateApplication(const CommandLineArgs& args) {
    ApplicationSpecification spec = {
        .name = "Raspberry",
        .min_width = 640,
        .min_height = 360,
        .enable_audio = true,
        .FPS = 0};

    Application* app = new Application(spec);
    app->GetLayerStack().PushLayer<Game>();
    return app;
}
