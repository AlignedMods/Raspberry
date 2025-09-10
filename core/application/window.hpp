#pragma once

#include "event/event.hpp"

#include <functional>

using EventCallbackFn = std::function<void(const Event&)>;

struct WindowData {
    std::string name;
    u32 width = 720, height = 1280;
};

// an abstract window class so we can use multiple windowing and input backends (GLFW, win32...)
// we also don't want to directly include any platform specific code in application.hpp
class Window {
public:
    Window(const WindowData& data) { m_WindowData = data; }
    virtual ~Window() = default;

    virtual bool ShouldClose() const = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRenderStart() = 0;
    virtual void OnRenderFinish() = 0;

    virtual f64 GetTime() const = 0;
    virtual void SleepMilli(i32 milliseconds) const = 0;
    void SleepSeconds(f64 seconds) const { SleepMilli(static_cast<i32>(seconds * 1000.0)); }

    virtual void* GetHandle() const = 0;

protected:
    EventCallbackFn m_EventCallback;
    WindowData m_WindowData;
};
