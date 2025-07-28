#include "game.hpp"
#include "core/log.hpp"
#include "renderer/renderer.hpp"

// not particuarly a huge fan of this
s_Game Game;
s_Renderer Renderer;

int main(int argc, char** argv) {
    // screw RAII btw
    if (!Game.Init()) {
        Log(LogLevel::Critical, "Could NOT initialize the game!\nAborting...");
        return 1;
    }

    // Main application loop
    while (Game.Running()) {
        Game.PollEvents();
        Game.OnUpdate();

        Renderer.Begin();

        Game.OnRender();

        Renderer.End();

        Game.CalculateTiming();
    }

    Game.Shutdown();

	return 0;
}
