#pragma once

#include "builder/editor/editor.hpp"
#include "renderer/renderer.hpp"

class Builder {
public:
	Builder();
	~Builder();

	void Run();

	static Builder* Get();
	static Renderer* GetRenderer();

private:
	Editor* m_Editor = nullptr;
};
