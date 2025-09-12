#include "application/entrypoint.hpp"
#include "game.hpp"

Blackberry::Application* Blackberry::Application::CreateApplication(const CommandLineArgs& args) {
    ApplicationSpecification spec = {
        .name = "Raspberry",
        .width = 1280,
        .height = 720,
        .min_width = 640,
        .min_height = 360,
        .enable_audio = true,
        .FPS = 240};

    Application* app = new Application(spec);
    app->GetLayerStack().PushLayer<Game>();
    return app;
}
