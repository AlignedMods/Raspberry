#pragma once

#include "collectable.hpp"

class Raspberry : public Collectable {
public:
    void Update();
    virtual void InitTextures() override;

    virtual void OnTouch() override;
    virtual void OnUpdate() override;
};
