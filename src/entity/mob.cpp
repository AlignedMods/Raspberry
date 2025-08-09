#include "mob.hpp"
#include "renderer/renderer.hpp"
#include "game.hpp"
#include "core/random.hpp"

#include "raymath.h"

void Mob::InitTextures() {
	m_Textures["IdleLeft"] = LoadTexture("Assets/Textures/brick.png");
	m_Textures["IdleRight"] = LoadTexture("Assets/entity/player_right.png");

	m_Textures["RunningLeftUp"] = LoadTexture("Assets/entity/player_left_walk_up.png");
	m_Textures["RunningLeftDown"] = LoadTexture("Assets/entity/player_left_walk_down.png");
	m_Textures["RunningRightUp"] = LoadTexture("Assets/entity/player_right_walk_up.png");
	m_Textures["RunningRightDown"] = LoadTexture("Assets/entity/player_right_walk_down.png");

	m_Texture = m_Textures.at("IdleLeft");
}

void Mob::OnUpdate() {
	Player& player = Game.GetCurrentLevel()->GetPlayer();

	if (player.GetPos().x > m_Position.x) {
		m_WalkSpeed.x = 3.0f;
	} else {
		m_WalkSpeed.x = -3.0f;
	}

	if (player.GetPos().y > m_Position.y) {
		m_WalkSpeed.y = 3.0f;
	} else {
		m_WalkSpeed.y = -3.0f;
	}

	m_Velocity.x = m_WalkSpeed.x + m_DashSpeed.x;
	m_Velocity.y = m_WalkSpeed.y + m_DashSpeed.y;

	m_Position = Vector2Add(m_Position, Vector2Scale(m_Velocity, Game.deltaTime));

	m_Texture = m_Textures.at("IdleLeft");
}

void Mob::OnRender() {
	Renderer.RenderEntity(m_Texture, m_Position);
}
