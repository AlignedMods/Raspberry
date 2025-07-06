// once upon a time i used raygui for this project... those days are over
//
// Now i be making my own gui system...
//
// Character development
//
// ################################
//
// CODE PAST THIS POINT! CODE PAST THIS POINT! CODE PAST THIS POINT!
// CODE PAST THIS POINT! CODE PAST THIS POINT! CODE PAST THIS POINT!
// CODE PAST THIS POINT! CODE PAST THIS POINT! CODE PAST THIS POINT!
//
// ################################

#pragma once

#include "shared/types.hpp"

#include <functional>
#include <string>

namespace Info {
    inline Font CurrentFont = { 0 };
    inline float Spacing = 2.0f;
} // namespace Info;

class Button {
public:
    //Button constructor
    Button(const Vector2& position, const Vector2& size, float fontSize,const std::string& text, bool useScale);
    Button(const Vector2& position, const Vector2& size, float fontSize,const std::string& text, bool useScale, std::function<void()> callback);

    void SetCallback(std::function<void()> callback);

    // Functions used for rendering and updating thy button
    void OnRender();
    void OnUpdate();

private:
    // functions

    float GetTextWidth();
    float GetTextHeight();

    float GetTextWidth(float customFontSize);

    // General variables
    Vector2 m_Position;
    Vector2 m_Size;

    Rectangle m_Rectangle;
    std::string m_Text;
    float m_FontSize;

    std::function<void()> m_Callback;

    bool m_IsCallbackSet = false;

    // Rendering variables
    bool m_UseScale;
    bool m_Hovered = false;
    bool m_Clicked = false;
};
