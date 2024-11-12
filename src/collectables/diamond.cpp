#include "diamond.hpp"
#include "game.hpp"
#include "raylib.h"

void Diamond::InitTextures()
{
    Image image = LoadImage("Assets/collectables/diamond.png");

    m_Texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void Diamond::Update()
{
    OnUpdate();
}

void Diamond::OnTouch()
{
    Game::GetCurrentLevel()->SetFound();
}

void Diamond::OnUpdate()
{
    if (CheckCollisionRecs(
            { Game::GetCurrentLevel()->GetPlayer()->GetX(),
                Game::GetCurrentLevel()->GetPlayer()->GetY(), 64, 64 },
            { m_Pos.x * 64, m_Pos.y * 64, 64, 64 })) {
        OnTouch();
    }
}
