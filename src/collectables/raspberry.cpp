#include "collectables/raspberry.hpp"
#include "game.hpp"

#include "raylib.h"

void Raspberry::InitTextures() {
  Image image = LoadImage("Assets/collectables/raspberry.png");

  m_Texture = LoadTextureFromImage(image);
  UnloadImage(image);
}

void Raspberry::Update() {
  OnUpdate();
}

void Raspberry::OnTouch() {
  Game.GetCurrentLevel().SetFound();
}

void Raspberry::OnUpdate() {
  if (CheckCollisionRecs({Game.GetCurrentLevel().GetPlayer().GetX(),
                          Game.GetCurrentLevel().GetPlayer().GetY(), 64, 64},
                         {m_Pos.x * 64, m_Pos.y * 64, 64, 64})) {
    OnTouch();
  }
}
