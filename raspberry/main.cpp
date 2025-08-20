#include "game.hpp"
#include "log.hpp"
#include "registry/registry.hpp"
#include "renderer/renderer.hpp"

// not particuarly a huge fan of this
// i think it's better than inline variable though
s_Game Game;
s_Renderer Renderer;
s_Registry Registry;

int main(int argc, char** argv) {
    // screw RAII btw
    if (!Game.Init()) {
        Log(Log::Critical, "Could NOT initialize the game!\nAborting...");
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
