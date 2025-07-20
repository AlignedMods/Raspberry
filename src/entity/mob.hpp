#pragma once

#include "entity.hpp"

class Mob : public Entity {
public:
	virtual void InitTextures() override;

	virtual void OnUpdate() override;
	virtual void OnRender() override;
};
