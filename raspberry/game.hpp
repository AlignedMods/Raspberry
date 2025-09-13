#pragma once

#include "application/layer.hpp"
#include "gui/menu.hpp"
#include "registry/registry.hpp"
#include "types.hpp"

#include <array>

class Game : public Blackberry::Layer {
public:
    Game();
    ~Game();

    virtual void OnInit() override;
    virtual void OnUpdate(f32 ts) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnUIRender(f32 ts) override;
    virtual void OnEvent(const Blackberry::Event& event) override;

    void SetCurrentLevel(const Level& level);

    void StartGameplay();

    void SetPause(bool yes);

    Level* GetCurrentLevel();

public:
    bool m_GameRunning = false;

private:
    Level* m_CurrentLevel = nullptr;
    
    bool m_Paused = false;

    Texture m_Fox;
};
