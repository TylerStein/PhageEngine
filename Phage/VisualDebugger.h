#pragma once
#include "imgui/imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#include "GLFW/glfw3.h"
#include <vector>
#include <string>

class VisualDebugger
{
public:
	static VisualDebugger* instance() {
		if (_instance == nullptr) {
			_instance = new VisualDebugger();
		}
		return _instance;
	}

	void Initialize(GLFWwindow* window);
	void UpdateTicks(double thisTick);

	void PreRender();
	void Render();
	void PostRender();

	bool _displayFPS;
	bool _displayTickRate;

private:
	static VisualDebugger* _instance;

	VisualDebugger();
	~VisualDebugger();

	//Calculated rendering/frame rate
	double _tickRate;

	//Total frames passed
	unsigned int _frames;

	//The last time of update
	unsigned int _lastTime;

};

/*
namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

}*/