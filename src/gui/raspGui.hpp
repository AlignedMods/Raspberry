// once upon a time i used raygui for this project... those days are over
//
// Now i be making my own gui system...
//
// Charatcer development
//
// ################################
//
// CODE PAST THIS POINT! CODE PAST THIS POINT! CODE PAST THIS POINT!
// CODE PAST THIS POINT! CODE PAST THIS POINT! CODE PAST THIS POINT!
// CODE PAST THIS POINT! CODE PAST THIS POINT! CODE PAST THIS POINT!
//
// ################################

#pragma once

#include "utils/types.hpp"

#include "pch.hpp"

class Button {
public:
    //Button constructor
    Button(const Vec2d& position, const Vec2d& size, const std::string& text, bool useScale);
    Button(const Vec2d& position, const Vec2d& size, const std::string& text, bool useScale, std::function<void()> callback);

    void SetCallback(std::function<void()> callback);

    // Functions used for rendering and updating thy button
    void OnRender();
    void OnUpdate();

private:
    // General variables
    Vec2d m_Position;
    Vec2d m_Size;

    Rec2d m_Rectangle;
    std::string m_Text;

    std::function<void()> m_Callback;

    bool m_IsCallbackSet = false;

    // Rendering variables
    bool m_UseScale;
    bool m_Hovered = false;
    bool m_Clicked = false;
};