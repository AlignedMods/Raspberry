// chat GPT our lord and saviour

#include "player.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "renderer/renderer.hpp"
#include "core/types.hpp"

#include "raylib.h"
#include "raymath.h"

#include <algorithm>

Player::Player() {
	m_Position.x = 0.0f;
	m_Position.y = 0.0f;

	m_Acceleration = 1;
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

void Player::OnUpdate() {
	m_Walking = false;

	if (IsKeyDown(KEY_D)) {
		m_Walking = true;
		m_Direction = 1;
		m_Velocity.x = std::min(320.0f, m_Velocity.x + 80.0f);
	}

	if (IsKeyDown(KEY_A)) {
		m_Walking = true;
		m_Direction = 0;
		m_Velocity.x = std::max(-320.0f, m_Velocity.x - 80.0f);
	}

	if (!m_Walking) {
		if (m_Velocity.x > 0.0f) {
			m_Velocity.x = std::max(0.0f, m_Velocity.x - 70.0f);
		}

		if (m_Velocity.x < 0.0f) {
			m_Velocity.x = std::min(0.0f, m_Velocity.x + 70.0f);
		}
	}

	m_Position = Vector2Add(Vector2Scale(m_Velocity, Game.deltaTime), m_Position);

    CheckCollisions();
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

void Player::CheckCollisions() {
    bool collision = false;

	Level& level = Game.GetCurrentLevel();

    for (auto& tile : level.GetAllTiles()) {
        //if ((m_Position.x + 64 > tile->GetPosition().x * 64 && m_Position.x < tile->GetPosition().x * 64 + 64)
			//&& (m_Position.y + 64 > tile->GetPosition().y * 64 && m_Position.y < tile->GetPosition().y * 64 + 64)) {
            //collision = true;
        //}
    }

    if (collision) {
        
    }
}

void Player::OnRender() {
    Renderer.RenderEntity(m_Texture, m_Position, m_TextureFlip);
}
