#pragma once

#include "event/event.hpp"

#include <sstream>

class MouseButtonPressedEvent : public Event {
public:
    MouseButtonPressedEvent(const MouseButton button)
        : m_Button(button) {}

    MouseButton GetButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategory_Input | EventCategory_Mouse);
    EVENT_CLASS_TYPE(MouseButtonPressed);

private:
    MouseButton m_Button;
};

class MouseButtonReleasedEvent : public Event {
public:
    MouseButtonReleasedEvent(const MouseButton button)
        : m_Button(button) {}

    MouseButton GetButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategory_Input | EventCategory_Mouse);
    EVENT_CLASS_TYPE(MouseButtonReleased);

private:
    MouseButton m_Button;
};

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(const u32 x, const u32 y)
        : m_X(x), m_Y(y) {}

    u32 GetX() const { return m_X; }
    u32 GetY() const { return m_Y; }

    virtual std::string ToString() const override {
        std::stringstream ss;
        ss << GetName() << ": " << m_X << ", " << m_Y;

        return ss.str();
    }

    EVENT_CLASS_CATEGORY(EventCategory_Input | EventCategory_Mouse);
    EVENT_CLASS_TYPE(MouseMoved);

private:
    u32 m_X;
    u32 m_Y;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(const f32 scroll)
        : m_Scroll(scroll) {}

    f32 GetScroll() const { return m_Scroll; }

    EVENT_CLASS_CATEGORY(EventCategory_Input | EventCategory_Mouse);
    EVENT_CLASS_TYPE(MouseScrolled);

private:
    f32 m_Scroll;
};
