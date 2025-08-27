#pragma once

#include "types.hpp"

#include <string>
#include <vector>

class Layer {
  public:
    Layer();
    virtual ~Layer() = default;

    std::string& GetName();
    void SetName(const std::string& name);

    // overridable functions
    virtual void OnInit() {}           // gets called when layer is first created
    virtual void OnRender() {}         // gets called each time the frame is rendered
    virtual void OnUIRender(f32 ts) {} // gets called when the ImGui UI is being rendered
    virtual void OnUpdate(f32 ts) {}   // gets called each time when the frame is being updated
    virtual void OnFixedUpdate() {}    // gets called exactly 60 times per second

  private:
    std::string m_Name;
};