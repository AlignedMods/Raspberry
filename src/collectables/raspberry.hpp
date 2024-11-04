#pragma once

#include "collectable.hpp"

class Raspberry : public Collectable {
public:
	Raspberry();
	void Update();
	void InitTextures();

	virtual void OnTouch() override;
	virtual void OnUpdate() override;
};
