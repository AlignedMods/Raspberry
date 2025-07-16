// chat GPT our lord and saviour

#include "player.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "renderer/renderer.hpp"
#include "core/types.hpp"
#include "core/log.hpp"

#include "raylib.h"
#include "raymath.h"

#include <algorithm>

Player::Player() {
	m_Position.x = 0.0f;
	m_Position.y = 0.0f;
}

void Player::InitTextures() {
	m_Textures["IdleLeft"] = LoadTexture("Assets/entity/player_left.png");
	m_Textures["RunningLeftUp"] = LoadTexture("Assets/entity/player_left_walk_up.png");
	m_Textures["RunningLeftDown"] = LoadTexture("Assets/entity/player_left_walk_down.png");

	m_Textures["IdleRight"] = LoadTexture("Assets/entity/player_right.png");
	m_Textures["RunningRightUp"] = LoadTexture("Assets/entity/player_right_walk_up.png");
	m_Textures["RunningRightDown"] = LoadTexture("Assets/entity/player_right_walk_down.png");

	m_Texture = m_Textures.at("IdleLeft");
}

Player Player::operator=(const Player& other) {
	return other;
}

// phyics
void Player::OnUpdate() {
	m_Walking = false;

	m_WalkSpeed.x = Approach(0.0f, m_WalkSpeed.x, 50.0f * Game.deltaTime);
	m_DashSpeed.x = Approach(0.0f, m_DashSpeed.x, 50.0f * Game.deltaTime);

	if (IsKeyDown(KEY_D)) {
		m_Walking = true;
		m_Direction = 1;
		m_WalkSpeed.x = 5.0f;
	}

	if (IsKeyDown(KEY_A)) {
		m_Walking = true;
		m_Direction = 0;
		m_WalkSpeed.x = -5.0f;
	}

	// Decrease the cooldowns
	m_JumpCooldown -= Game.deltaTime;

	m_Dashing = false;

	if (IsKeyDown(KEY_E)) {
		if (GetTime() - m_TimeSinceDash > 3.0) {
			if (m_Direction) {
				Log(LogLevel::Info, "right");
				m_DashSpeed.x = 20.0f;
			} else {
				Log(LogLevel::Info, "left");
				m_DashSpeed.x = -20.0f;
			}

			m_Dashing = true;
			m_TimeSinceDash += 3.0;
		}
	}

	m_Velocity.x = m_WalkSpeed.x + m_DashSpeed.x;

	if (m_Grounded) {
		m_CoyoteTimeCounter = 0.15;
	} else {
		m_CoyoteTimeCounter -= Game.deltaTime;
	}

	if (IsKeyPressed(KEY_SPACE)) {
		if (m_CoyoteTimeCounter > 0.0) {
			m_Velocity.y = -9.0f;
			m_Grounded = false;
		}

		m_JumpCooldown += 0.5f * Game.deltaTime;
	}

	m_Velocity.y += m_Gravity * Game.deltaTime;

	m_Position.x += m_Velocity.x * Game.deltaTime;
	CheckCollisionsH();

	m_Position.y += m_Velocity.y * Game.deltaTime;
	CheckCollisionsV();
}

float Player::Approach(float end, float current, float interval) {
	float val = current;

	if (end > current) {
		val = std::min(current + interval, end);
	} else {
		val = std::max(current - interval, end);
	}

	return val;
}

void Player::UpdateTextures() {
    m_Anim++;

    if (m_Direction == 0) {
        m_Texture = m_Textures.at("IdleLeft");

        if (m_Walking && m_Velocity.x < 0.0f) {
            if (m_Anim % 10 > 5) {
                m_Texture = m_Textures.at("RunningLeftUp");
            } else {
                m_Texture = m_Textures.at("RunningLeftDown");
            }
        }
    }

    if (m_Direction == 1) {
        m_Texture = m_Textures.at("IdleRight");

        if (m_Walking && m_Velocity.x > 0.0f) {
            if (m_Anim % 10 > 5) {
                m_Texture = m_Textures.at("RunningRightUp");
            } else {
                m_Texture = m_Textures.at("RunningRightDown");
            }
        }
	}
}

void Player::CheckCollisionsH() {
	Level& level = Game.GetCurrentLevel();

    for (auto& tile : level.GetAllTiles()) {
		if (CheckCollisionRecs({m_Position.x, m_Position.y + 0.1f, 1.0f, 0.9f},
							   {tile.GetPosition().RaylibVector().x, tile.GetPosition().RaylibVector().y, 1.0f, 1.0f})) 
		{
			if (m_Velocity.x > 0) {
				m_Position.x = tile.GetPosition().RaylibVector().x - 1.0f;
			} else if (m_Velocity.x < 0) {
				m_Position.x = tile.GetPosition().RaylibVector().x + 1.0f;
			}

			m_Velocity.x = 0;
		}
    }
}

void Player::CheckCollisionsV() {
	Level& level = Game.GetCurrentLevel();

	m_Grounded = false;

    for (auto& tile : level.GetAllTiles()) {
		if (CheckCollisionRecs({m_Position.x, m_Position.y + 0.1f, 1.0f, 0.9f},
							   {tile.GetPosition().RaylibVector().x, tile.GetPosition().RaylibVector().y, 1.0f, 1.0f})) 
		{
			if (m_Velocity.y > 0) {
				m_Position.y = tile.GetPosition().RaylibVector().y - 1.0f;
				m_Grounded = true;
			} else if (m_Velocity.y < 0) {
				m_Position.y = tile.GetPosition().RaylibVector().y + 1.0f;
			}

			m_Velocity.y = 0;
		}
    }
}

void Player::OnRender() {
    Renderer.RenderEntity(m_Texture, m_Position, m_TextureFlip);
}
