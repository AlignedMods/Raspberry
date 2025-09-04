#include "mob.hpp"
#include "game.hpp"
#include "registry/registry.hpp"

#include "raymath.h"

void Mob::InitTextures() {
    // m_Textures["IdleLeft"] = Registry.GetTexture("brick");
    // m_Textures["IdleRight"] = Registry.GetTexture("brick");

    // m_Textures["RunningLeftUp"] = Registry.GetTexture("stone");
    // m_Textures["RunningLeftDown"] = Registry.GetTexture("stone");
    // m_Textures["RunningRightUp"] = Registry.GetTexture("stone");
    // m_Textures["RunningRightDown"] = Registry.GetTexture("stone");

    // m_Texture = m_Textures.at("IdleLeft");
}

void Mob::OnUpdate() {
    // Player& player = Game.GetCurrentLevel()->GetPlayer();

    // if (player.GetPos().x > m_Position.x) {
    // 	m_WalkSpeed.x = 3.0f;
    // } else {
    // 	m_WalkSpeed.x = -3.0f;
    // }

    // if (player.GetPos().y > m_Position.y) {
    // 	m_WalkSpeed.y = 3.0f;
    // } else {
    // 	m_WalkSpeed.y = -3.0f;
    // }

    // m_Velocity.x = m_WalkSpeed.x + m_DashSpeed.x;
    // m_Velocity.y = m_WalkSpeed.y + m_DashSpeed.y;

    // m_Position = Vector2Add(m_Position, Vector2Scale(m_Velocity, Game.deltaTime));

    // m_Texture = m_Textures.at("IdleLeft");
}

void Mob::OnRender() {
    // Renderer.RenderEntity(m_Texture, m_Position);
}
