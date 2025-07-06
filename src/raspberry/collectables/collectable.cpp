#include "raspberry/collectables/collectable.hpp"
#include "shared/types.hpp"
#include "shared/random.hpp"
#include "raspberry/game.hpp"
#include "shared/log.hpp"

#include "raylib.h"

Collectable::Collectable() {
  m_Pos = Random::Vec2(0, 25);

  Debug(m_Pos.x);
  Debug(m_Pos.y);
}

Collectable::~Collectable() {
  Debug("DESTROYING COLLECTABLE!");
  UnloadTexture(m_Texture);
}

void Collectable::Draw() {
  Renderer.RenderTexture(m_Texture, m_Pos.x * 64, m_Pos.y * 64);
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
  Game.GetCurrentLevel()->SetFound();
}

void Collectable::OnUpdate() {
  if (CheckCollisionRecs({Game.GetCurrentLevel()->GetPlayer()->GetX(),
                          Game.GetCurrentLevel()->GetPlayer()->GetY(), 64, 64},
                         {m_Pos.x * 64, m_Pos.y * 64, 64, 64})) {
    OnTouch();
  }
}
