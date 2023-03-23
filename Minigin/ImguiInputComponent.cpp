#include "ImguiInputComponent.h"
#include "ImguiWrapper.h"
#include "InputManager.h"

void dae::ImguiInputComponent::DrawUI()
{
	ImGui::Begin("Input Debug Window");

	auto gamepads = InputManager::GetInstance().GetGamepads();

	for (size_t i = 0; i < 4; i++)
	{
		ImGui::Text("Gamepad %d is", i);
		ImGui::SameLine();
		if (gamepads[i].connected)
		{
			ImGui::Text("connected.");
		}
		else
		{
			ImGui::Text("not connected.");
		}
	}


	ImGui::End();
}
