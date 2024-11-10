#pragma once

#include "graphics/renderer.hpp"
#include "level/level.hpp"

class Game {
public:
  Game();
  ~Game();

  void Run();

  void SetCurrentLevel(Level *level);
  Level *GetCurrentLevel();

  void StartGameplay();

  static Game *Get();
  static Renderer *GetRenderer();

private:
  Level *m_CurrentLevel = nullptr;

  bool m_IsGamePlayRunning = false;
};
