#pragma once

#include <pch.hpp>

#include "graphics/renderer.hpp"
#include "level/level.hpp"
#include "menu/menu.hpp"

using std::shared_ptr;

class Game {
public:
    Game();
    ~Game();

    void Run();
    void Loop(Menu& menu, Camera2D& camera);
    void Tick();

    void SetCurrentLevel(std::shared_ptr<Level> level);

    void StartGameplay();

    static Game* Get();
    static shared_ptr<Renderer> GetRenderer();

    static shared_ptr<Level> GetCurrentLevel();

private:
    shared_ptr<Level> m_CurrentLevel = nullptr;

    bool m_isGamePlayRunning = false;
  
public:
    float m_LastTime;
    float m_Time = 0.0f;
};
