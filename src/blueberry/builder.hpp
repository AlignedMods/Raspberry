#pragma once

#include "blueberry/editor/editor.hpp"

#include "raylib.h"

class Builder {
public:
	Builder();
	~Builder();

	void Run();

	static Builder* Get();

    void LoadEditor();

private:
	Editor* m_Editor = nullptr;

    Camera2D m_Camera = { 0 };
};
