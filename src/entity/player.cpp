// chat GPT our lord and saviour

#include "player.hpp"
#include "game.hpp"
#include "registry.hpp"
#include "renderer/renderer.hpp"
#include "core/log.hpp"

#include "raylib.h"

void Player::InitTextures() {
	m_Textures["IdleLeft"] = Registry.GetTexture("player_left");
	m_Textures["RunningLeftUp"] = Registry.GetTexture("player_left_walk_up");
	m_Textures["RunningLeftDown"] = Registry.GetTexture("player_left_walk_down");

	m_Textures["IdleRight"] = Registry.GetTexture("player_right");
	m_Textures["RunningRightUp"] = Registry.GetTexture("player_right_walk_up");
	m_Textures["RunningRightDown"] = Registry.GetTexture("player_right_walk_down");

	m_Texture = m_Textures.at("IdleLeft");
}

// phyics
void Player::OnUpdate() {
	m_Texture = Registry.GetTexture("player_left");

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

    if (IsGamepadAvailable(0)) {
        f32 axisX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);

        if (std::abs(axisX) > 0.1f) {
            m_WalkSpeed.x = axisX * 5.0f;
        }

        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
			m_Velocity.y = -9.0f;
			m_Grounded = false;

		    m_JumpCooldown += 0.5f * Game.deltaTime;
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
