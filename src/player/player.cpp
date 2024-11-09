#include "player.hpp"
#include "raylib.h"
#include "game.hpp"

Player::Player() {
	m_X = 0.0f;
	m_Y = 0.0f;

	m_Size = 64.0f;
	m_Speed = 0.5f;

	m_IsJumping = false;

	m_Velocity = {0.0f, 0.0f};

	m_Level = Game::Get()->GetCurrentLevel();
}

void Player::Update() {
	if (!m_IsOnGround) {
		m_Velocity.y += m_Gravity;
	}

	m_X += m_Velocity.x;
	m_Y += m_Velocity.y;

	if (IsKeyDown(KEY_D)) {
		m_Velocity.x += m_Speed;
	} else if (IsKeyDown(KEY_A)) {
		m_Velocity.x -= m_Speed;
	} else {
		if (m_Velocity.x < 0) {
			m_Velocity.x += m_Speed;
		} else if (m_Velocity.x > 0) {
			m_Velocity.x -= m_Speed;
		}
	}

	if (IsKeyPressed(KEY_SPACE)) {
		Jump();
	}

	CheckCollisions();
}

void Player::CheckCollisions() {
	auto tiles = m_Level->GetAllTiles();

	// Flags to track different collision states
	bool isGroundCollision = false;
	bool isLeftCollision = false;
	bool isRightCollision = false;	

	Vector2 finalPos = {m_X, m_Y};

	for (auto rec : tiles) {
		if (CheckCollisionRecs({m_X, m_Y, m_Size, m_Size}, {rec.GetX(), rec.GetY(), rec.GetSize(), rec.GetSize()})) {
			// Collision from the top
			if (m_Velocity.y > 0) {
				finalPos.y = rec.GetY() - m_Size;
				m_Velocity.y = 0;
				isGroundCollision = true;
				m_IsJumping = false;
			}
		}
	}

	m_Y = finalPos.y;
	m_IsOnGround = isGroundCollision;

	// Check horizontal collisions separately
	for (auto rec : tiles) {
		// Only check for horizontal collisions if the player is on the ground (or if that's your intended behavior)
		if (CheckCollisionRecs({m_X, m_Y, m_Size, m_Size}, {rec.GetX(), rec.GetY(), rec.GetSize(), rec.GetSize()})) {
			// Going right
			if (m_Velocity.x > 0 && m_X + m_Size >= rec.GetX()) {
				m_Velocity.x = 0;
				finalPos.x = rec.GetX() - m_Size;
				isRightCollision = true;
			} 
			// Going left
			else if (m_Velocity.x < 0 && m_X <= rec.GetX() + rec.GetSize()) {
				m_Velocity.x = 0;
				finalPos.x = rec.GetX() + rec.GetSize();
				isLeftCollision = true;
			}
		}
	}

	m_X = finalPos.x;
}

float Player::GetX() {
	return m_X;
}

float Player::GetY() {
	return m_Y;
}

void Player::Draw() {
	DrawRectangle(m_X, m_Y, m_Size, m_Size, RED);
}

void Player::Jump() {
	if (m_IsOnGround || IsTileBelowPlayer()) {
		m_Velocity.y -= 10.0f;
		m_IsJumping = true;
		m_IsOnGround = false;
	}
}

bool Player::IsTileBelowPlayer() {
	auto tiles = m_Level->GetAllTiles();

	for (auto tile : tiles) {
		if (tile.GetY() == m_Y + m_Size - 0.5f) {
			return true;
		}
	}

	return false;
}
