// chat GPT our lord and saviour

#include "player.hpp"
#include "game.hpp"
#include "renderer/renderer.hpp"
#include "core/log.hpp"

#include "raylib.h"

void Player::InitTextures() {
	m_Textures["IdleLeft"] = LoadTexture("Assets/entity/player_left.png");
	m_Textures["RunningLeftUp"] = LoadTexture("Assets/entity/player_left_walk_up.png");
	m_Textures["RunningLeftDown"] = LoadTexture("Assets/entity/player_left_walk_down.png");

	m_Textures["IdleRight"] = LoadTexture("Assets/entity/player_right.png");
	m_Textures["RunningRightUp"] = LoadTexture("Assets/entity/player_right_walk_up.png");
	m_Textures["RunningRightDown"] = LoadTexture("Assets/entity/player_right_walk_down.png");

	m_Texture = m_Textures.at("IdleLeft");
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

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		if (GetTime() - m_TimeSinceDash > 3.0) {
			if (m_Direction) {
				Log(LogLevel::Info, "right");
				m_DashSpeed.x = 20.0f;
			} else {
				Log(LogLevel::Info, "left");
				m_DashSpeed.x = -20.0f;
			}

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
		#ifndef RDEBUG
		if (m_CoyoteTimeCounter > 0.0) {
            m_CoyoteTimeCounter = 0.0;
			m_Velocity.y = -9.0f;
			m_Grounded = false;
            m_TimesJumped = 1;
		} else if (m_TimesJumped < 2) {
            m_Velocity.y = -9.0f;
            m_Grounded = false;
            m_TimesJumped = 2;
        }
		#else
			m_Velocity.y = -9.0f;
			m_Grounded = false;
		#endif

		m_JumpCooldown += 0.5f * Game.deltaTime;
	}

	m_Velocity.y += m_Gravity * Game.deltaTime;

	OffsetPositionByVelocity();
}


void Player::OnRender() {
    Renderer.RenderEntity(m_Texture, m_Position);
}
