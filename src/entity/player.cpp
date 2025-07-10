// chat GPT our lord and saviour

#include "player.hpp"
#include "entity/entity.hpp"
#include "game.hpp"
#include "renderer/renderer.hpp"
#include "core/types.hpp"

#include "raylib.h"

#include <algorithm>

Player::Player() {
	m_Position.x = 0.0f;
	m_Position.y = 0.0f;

	m_Acceleration = 1;

	m_State = EntityState::Idle;
}

void Player::InitTextures() {
	m_Textures["IdleFront"] = LoadTexture("Assets/entity/player_front.png");
	m_Textures["RunningFrontLeft"] = LoadTexture("Assets/entity/player_front_walk_left.png");
	m_Textures["RunningFrontRight"] = LoadTexture("Assets/entity/player_front_walk_right.png");

	m_Textures["IdleBack"] = LoadTexture("Assets/entity/player_back.png");
	m_Textures["RunningBackLeft"] = LoadTexture("Assets/entity/player_back_walk_left.png");
	m_Textures["RunningBackRight"] = LoadTexture("Assets/entity/player_back_walk_right.png");

	m_Textures["IdleLeft"] = LoadTexture("Assets/entity/player_left.png");
	m_Textures["RunningLeftUp"] = LoadTexture("Assets/entity/player_left_walk_up.png");
	m_Textures["RunningLeftDown"] = LoadTexture("Assets/entity/player_left_walk_down.png");

	m_Texture = m_Textures.at("IdleFront");
}

Player Player::operator=(const Player& other) {
	return other;
}

void Player::OnUpdate() {
    if (m_State == EntityState::Freefalling) {
        m_Velocity.y += m_Gravity;
    }

    if (IsKeyDown(KEY_W)) {
        m_State = EntityState::Running;
        m_Direction = Direction::Up;

        m_Velocity.y = std::max(m_Velocity.y - m_Acceleration, -10.0f);
    }

    if (IsKeyDown(KEY_S)) {
        m_State = EntityState::Running;
        m_Direction = Direction::Down;
        m_Velocity.y = std::min(m_Velocity.y + m_Acceleration, 10.0f);
    }

    if (IsKeyDown(KEY_D)) {
        m_State = EntityState::Running;
        m_Direction = Direction::Right;
        m_Velocity.x = std::min(m_Velocity.x + m_Acceleration, 10.0f);
    } 

    if (IsKeyDown(KEY_A)) {
        m_State = EntityState::Running;
        m_Direction = Direction::Left;
        m_Velocity.x = std::max(m_Velocity.x - m_Acceleration, -10.0f);
    }

    if (IsKeyUp(KEY_W)) {
        if (m_Velocity.y < 0.0f) {
            m_Velocity.y = std::min(m_Velocity.y + m_Acceleration * 2, 0.0f);
        }
    }
    
    if (IsKeyUp(KEY_S)) {
        if (m_Velocity.y > 0.0f) {
            m_Velocity.y = std::max(m_Velocity.y - m_Acceleration * 2, 0.0f);
        }
    }

    if (IsKeyUp(KEY_A)) {
        if (m_Velocity.x < 0.0f) {
            m_Velocity.x = std::min(m_Velocity.x + m_Acceleration * 2, 0.0f);
        }
    }
    if (IsKeyUp(KEY_D)) {
        if (m_Velocity.x > 0.0f) {
            m_Velocity.x = std::max(m_Velocity.x - m_Acceleration * 2, 0.0f);
        }
    }

    if (IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) {
        m_State = EntityState::Idle;
    }

    m_Position.x += m_Velocity.x;
    m_Position.y += m_Velocity.y;

    CheckCollisions();
}

void Player::UpdateTextures() {
    m_Anim++;

    if (m_Direction == Direction::Down) {
        m_Texture = m_Textures.at("IdleFront");

        if (m_State == EntityState::Running && m_Velocity.y > 0.0f) {
            if (m_Anim % 5 > 2.5) {
                m_Texture = m_Textures.at("RunningFrontLeft");
            } else {
                m_Texture = m_Textures.at("RunningFrontRight");
            }
        }
    }

    if (m_Direction == Direction::Up) {
        m_Texture = m_Textures.at("IdleBack");
        m_TextureFlip = false;

        if (m_State == EntityState::Running) {
            if (m_Anim % 5 > 2.5) {
                m_Texture = m_Textures.at("RunningBackLeft");
            } else {
                m_Texture = m_Textures.at("RunningBackRight");
            }
        }
    }

    if (m_Direction == Direction::Left) {
        
        m_Texture = m_Textures.at("IdleLeft");
        m_TextureFlip = false;

        if (m_State == EntityState::Running && m_Velocity.x < 0.0f) {
            if (m_Anim % 5 > 2.5) {
                m_Texture = m_Textures.at("RunningLeftUp");
            } else {
                m_Texture = m_Textures.at("RunningLeftDown");
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
