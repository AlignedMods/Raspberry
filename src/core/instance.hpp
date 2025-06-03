#include "utils/types.hpp"

class Instance {
    public:
    virtual ~Instance() = default;

    virtual void OnRender() {}

    void OnInit();

    float m_ZOrder = 0.0f;
    Vec2d m_Position;
};