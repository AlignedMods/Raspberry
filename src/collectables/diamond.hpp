#pragma once

#include "collectable.hpp"

class Diamond : public Collectable {
public:
    void Update();
    void InitTextures() override;

    void OnUpdate() override;
    void OnTouch() override;
};
