#pragma once

#include "entity/entity.hpp"

#include <map>
#include <string>

class Player : public Entity {
public:
	void InitTextures() override;


    virtual void OnRender() override;
    virtual void OnUpdate() override;

private:
    int m_TimesJumped = 0;
};
