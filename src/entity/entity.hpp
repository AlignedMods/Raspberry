#pragma once

#include "tile/tile.hpp"
#include "core/types.hpp"

#include "raylib.h"

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

	bool m_Walking;
	bool m_Falling;

	int m_Direction;

	Texture m_Texture;

	int m_Anim = 0;
	bool m_TextureFlip = false;
};
