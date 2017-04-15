#include "VisualDebugger.h"

VisualDebugger* VisualDebugger::_instance = 0;

void VisualDebugger::Initialize(GLFWwindow * window)
{
	ImGui_ImplGlfwGL3_Init(window, true);
}

void VisualDebugger::UpdateTicks(double thisTick)
{
	_frames++;

	double diff = (thisTick - _lastTime);
	if (diff >= 1.0) {
		_tickRate = (float)(1000.0 / double(_frames));
		_frames = 0;
		_lastTime += diff;
	}
}

void VisualDebugger::PreRender()
{
	//Create IMGUI frame
	ImGui_ImplGlfwGL3_NewFrame();
}

void VisualDebugger::Render()
{


	if (_displayTickRate) {
		ImGui::Begin("Debug");
		{
			ImGui::Text("Render Time (ms): %f", _tickRate);
		}
		ImGui::End();
	}


}

void VisualDebugger::PostRender()
{
	//Render the IMGUI frame
	ImGui::Render();
}


VisualDebugger::VisualDebugger()
{
	_tickRate = 0;
	_frames = 0;
	_lastTime = 0;
	_displayFPS = false;
	_displayTickRate = false;
}


VisualDebugger::~VisualDebugger()
{
	//Shut down and clean up ImGui
	ImGui_ImplGlfwGL3_Shutdown();
}
