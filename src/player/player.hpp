#pragma once

#include "level/level.hpp"
#include "raylib.h"

enum class Direction {
	nowhere = 0, Up, Down, Right, Left, DownRight, DownLeft
};

class Level;

class Player {
public:
	Player();
	void Update();
	void Draw();
	float GetX();
	float GetY();

	void Jump();
	bool IsTileBelowPlayer();
private:
	void CheckCollisions();

	float m_X;
	float m_Y;
	float m_Size;
	float m_Speed;

	const float m_Gravity = 0.5f;
	const float m_GroundBuffer = 2.0f;

	Vector2 m_Velocity;

	bool m_IsJumping;
	bool m_IsOnGround;

	Texture2D m_Texture;
	Level* m_Level = nullptr;
};
