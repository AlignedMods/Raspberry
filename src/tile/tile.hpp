#pragma once

#include "core/types.hpp"

#include "raylib.h"

#include <string>

class Tile {
public:
	Tile() = default;
	Tile(const std::string& type, const i2& position);

	void OnRender();
	void OnRender(u32 flag);

	i2& GetPosition();
    f2& GetRealPosition();
	std::string& GetType();

public:
    f32 brightness = 1.0f;

private:
	i2 m_Position;
    f2 m_RealPosition;
	std::string m_Type;

	Texture m_Texture;
};
