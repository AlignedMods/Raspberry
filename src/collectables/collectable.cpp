#include "collectables/collectable.hpp"
#include "core/random.hpp"
#include "game.hpp"
#include "core/log.hpp"

#include "raylib.h"

#include <format>

Collectable::Collectable() {
  m_Pos = Random::Vec2(0, 25);
}

Collectable::~Collectable() {
  Debug("DESTROYING COLLECTABLE!");
  UnloadTexture(m_Texture);
}

void Collectable::OnRender() {
	Renderer.RenderTexture(m_Texture, m_Pos.x * 64, m_Pos.y * 64, WHITE);
}

Vector2& Collectable::GetTilePos() {
  return m_Pos;
}

void Collectable::InitTextures() {
  Image image = LoadImage("Assets/collectables/default.png");

  m_Texture = LoadTextureFromImage(image);
  UnloadImage(image);
}

void Collectable::Update() {
  OnUpdate();
}

void Collectable::OnTouch() {
  Game.GetCurrentLevel().SetFound();
}

void Collectable::OnUpdate() {
  if (CheckCollisionRecs({Game.GetCurrentLevel().GetPlayer().GetX(),
                          Game.GetCurrentLevel().GetPlayer().GetY(), 64, 64},
                         {m_Pos.x * 64, m_Pos.y * 64, 64, 64})) {
    OnTouch();
  }
}
