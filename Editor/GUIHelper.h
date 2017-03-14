#pragma once
#include "imgui\imgui.h"
#include "PhageEngine.h"
#include <vector>
#include <string>

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

}

namespace GuiHelper
{
	static void renderAssetList(char* label, int* select, std::vector<pAsset*> assetList) {
		std::vector<std::string> assetNames = std::vector<std::string>();
		for (int i = 0; i < assetList.size(); ++i) {
			assetNames.push_back(assetList.at(i)->getName());
		}

		ImGui::ListBox(label, select, assetNames);
	}
}
