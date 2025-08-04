#pragma once

#include "core/types.hpp"

// The Raspberry color palette™

namespace Colors {

// The RGB trilogy
inline Hex Red    = 0xf25757ff;
inline Hex Green  = 0x69f257ff;
inline Hex Blue   = 0x4287f5ff;

// The colorless
inline Hex Black  = 0x141414ff;
inline Hex DGray  = 0x3d3d3dff;
inline Hex Gray   = 0x696969ff;
inline Hex LGray  = 0x9c9c9cff;
inline Hex White  = 0xe0e0e0ff;

// Based off red
inline Hex Yellow = 0xfcfc4cff;
inline Hex Orange = 0xfca44cff;

// Based off blue
inline Hex Cyan   = 0x5edfffff;
inline Hex Purple = 0x954cfcff;

// Based off green
inline Hex Teal   = 0x55fac0ff;
inline Hex Lime   = 0x25cc51ff;

}

struct Style {
    Hex DefaultFill = Colors::DGray;
    Hex DefaultOutline = Colors::Teal;
    Hex HoveredFill = Colors::Gray;
    Hex HoveredOutline = Colors::Teal;
    Hex ClickedFill = Colors::LGray;
    Hex ClickedOutline = Colors::Teal;

    f32 Roundness = 0.0f;
    u32 Outline = 2;
};

// this is a base class for every gui element to inherit from
class GuiElement {
public:
    virtual void OnUpdate() {}
    virtual void OnRender() {}

protected:
    PosInfo m_Info;
    Style m_Style;
};
