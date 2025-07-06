#pragma once

#include "raspberry/tile/tile.hpp"
#include "shared/types.hpp"

#include "raylib.h"

enum class EntityState {
    Running,
    Jumping,
    Freefalling,
    Idle
};

enum class Direction {
    Up,
    Down,
    Right,
    Left
};

class Entity {
public:
	virtual ~Entity() {}
	virtual void OnUpdate() {}

	virtual void OnRender() {}

	float GetX();
	float GetY();

	Vector2& GetPos();

protected:
	Vector2 m_Velocity;
	Vector2 m_Position;

	EntityState m_State;

	Texture m_Texture;

	int m_Anim = 0;
	Direction m_Direction = Direction::Down;
	bool m_TextureFlip = false;
};
