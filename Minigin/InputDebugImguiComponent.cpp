#include "InputDebugImguiComponent.h"
#include "ImguiWrapper.h"
#include <format>
#include "glm\glm.hpp"
#include "InputSystem.h"
#include "InputDeviceGamepad.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"

#include "EventManager.h"

namespace dae::Input
{

	void InputDebugImguiComponent::OnEvent(const Event& sent)
	{
		switch (sent.GetEventType())
		{
			case EventType::OnDeath:
			++DeathCounter;
			default:
				break;
		}
	}

#pragma region BOILERPLATE

	namespace
	{
		static uint32_t const g_controlOutlineColor = 0xFF666666;
		static uint32_t const g_controlFillColor = 0xFF888888;

		static float const g_buttonWidth = 22;
		static float const g_buttonBorderThickness = 2.0f;
		static float const g_buttonBorderRounding = 10.0f;

		static float const g_analogStickRangeRadius = 50;
		static float const g_analogStickPositionRadius = 2;

		static ImVec2 const g_buttonBorderOffset(g_buttonBorderThickness, g_buttonBorderThickness);
		static ImVec2 const g_buttonDimensions(g_buttonWidth, g_buttonWidth);
		static ImVec2 const g_shoulderDimensions(g_buttonWidth * 1.7f, g_buttonWidth);
		static float const g_dpadStretchMultiplier(1.4f);
	}

	static void DrawButton(ImDrawList* pDrawList, const ImVec2& position, const ImVec2& dimensions, char const* const pLabel, bool IsHeldDown, uint32_t buttonColor = g_controlOutlineColor, uint32_t pressedColor = g_controlFillColor, bool noRounding = false)
	{
		assert(pDrawList != nullptr);
		float rounding = (noRounding) ? 0.5f : g_buttonBorderRounding;

		const ImVec2 buttonTopLeft = position;
		const ImVec2 buttonBottomRight = ImVec2(buttonTopLeft.x + dimensions.x, buttonTopLeft.y + dimensions.y);
		pDrawList->AddRect(buttonTopLeft, buttonBottomRight, buttonColor, rounding, ImDrawFlags_RoundCornersAll, g_buttonBorderThickness);

		if (IsHeldDown)
		{
			ImVec2 pMax = ImVec2(buttonTopLeft.x + dimensions.x - g_buttonBorderOffset.x, buttonTopLeft.y + dimensions.y - g_buttonBorderOffset.y);
			pDrawList->AddRectFilled(ImVec2(buttonTopLeft.x + g_buttonBorderOffset.x, buttonTopLeft.y + g_buttonBorderOffset.y), pMax, pressedColor, rounding, ImDrawFlags_RoundCornersAll);
		}

		if (pLabel != nullptr)
		{
			ImVec2 const textDimensions = ImGui::CalcTextSize(pLabel);
			ImVec2 const textPos = ImVec2(buttonTopLeft.x + (dimensions.x / 2) - (textDimensions.x / 2) + 1, buttonTopLeft.y + (dimensions.y / 2) - (textDimensions.y / 2));
			pDrawList->AddText(textPos, 0xFFFFFFFF, pLabel);
		}
	}

	static void DrawTriggerButton(ImDrawList* pDrawList, ImVec2 const& position, ImVec2 const& dimensions, char const* const pLabel, const InputStateGamepad& controllerState, bool isLeftTrigger)
	{
		assert(pDrawList != nullptr);

		// Draw the label if set
		ImVec2 drawPosition = position;
		if (pLabel != nullptr)
		{
			ImVec2 const textDimensions = ImGui::CalcTextSize(pLabel);
			ImVec2 const textPos = ImVec2(drawPosition.x + (dimensions.x / 2) - (textDimensions.x / 2) + 1, drawPosition.y + g_buttonBorderThickness);
			pDrawList->AddText(textPos, 0xFFFFFFFF, pLabel);
			drawPosition.y += textDimensions.y + 4;
		}

		// Draw the border
		ImVec2 const borderDimensions(dimensions.x, dimensions.y - (drawPosition.y - position.y + 4));
		ImVec2 const triggerTopLeft = drawPosition;
		ImVec2 const triggerBottomRight{ triggerTopLeft.x + borderDimensions.x,triggerTopLeft.y + borderDimensions.y };
		pDrawList->AddRect(triggerTopLeft, triggerBottomRight, g_controlOutlineColor, 0.0f, ImDrawFlags_RoundCornersAll, g_buttonBorderThickness);

		// Draw the trigger values
		float const triggerValueRaw = isLeftTrigger ? controllerState.GetLeftTriggerRawValue() : controllerState.GetRightTriggerRawValue();
		if (triggerValueRaw > 0)
		{
			float triggerValue0;
			uint32_t triggerValue0Color;

			float triggerValue1;
			uint32_t triggerValue1Color;

			if (isLeftTrigger)
			{
				triggerValue0 = controllerState.GetLeftTriggerRawValue();
				triggerValue1 = controllerState.GetLeftTriggerValue();
				triggerValue0Color = 0xFF0000FF;
				triggerValue1Color = 0xFF00FF00;
			}
			else
			{
				triggerValue0 = controllerState.GetRightTriggerValue();
				triggerValue1 = controllerState.GetRightTriggerRawValue();
				triggerValue0Color = 0xFF00FF00;
				triggerValue1Color = 0xFF0000FF;
			}

			float const valueMaxLength = borderDimensions.y - (g_buttonBorderThickness * 2);
			float const triggerValueWidth = (borderDimensions.x - g_buttonBorderThickness * 2) / 2;
			float const triggerValue0TopLeftX = drawPosition.x + g_buttonBorderThickness;
			float const triggerValue1TopLeftX = triggerValue0TopLeftX + triggerValueWidth;
			float const triggerValue0TopLeftY = drawPosition.y + g_buttonBorderThickness + (1.0f - triggerValue0) * valueMaxLength;
			float const triggerValue1TopLeftY = drawPosition.y + g_buttonBorderThickness + (1.0f - triggerValue1) * valueMaxLength;

			ImVec2 const triggerValue0TopLeft(triggerValue0TopLeftX, triggerValue0TopLeftY);
			ImVec2 const triggerValue0BottomRight(triggerValue1TopLeftX, triggerBottomRight.y - g_buttonBorderThickness);
			pDrawList->AddRectFilled(triggerValue0TopLeft, triggerValue0BottomRight, triggerValue0Color);

			ImVec2 const triggerValue1TopLeft(triggerValue0TopLeftX + triggerValueWidth, triggerValue1TopLeftY);
			ImVec2 const triggerValue1BottomRight(triggerValue1TopLeftX + triggerValueWidth, triggerBottomRight.y - g_buttonBorderThickness);
			pDrawList->AddRectFilled(triggerValue1TopLeft, triggerValue1BottomRight, triggerValue1Color);
		}
	}

	static void DrawAnalogStick(ImDrawList* pDrawList, ImVec2 const position, const InputDeviceGamepad& controller, bool isLeftStick)
	{
		assert(pDrawList != nullptr);

		auto const& settings = controller.GetSettings();
		auto const& controllerState = controller.GetGamepadState();

		glm::vec2 rawValue = isLeftStick ? controllerState.GetLeftAnalogStickRawValue() : controllerState.GetRightAnalogStickRawValue();
		glm::vec2 filteredValue = isLeftStick ? controllerState.GetLeftAnalogStickValue() : controllerState.GetRightAnalogStickValue();

		// Invert the y values to match screen space
		rawValue.y = -rawValue.y;
		filteredValue.y = -filteredValue.y;

		// Draw max stick range and dead zone range
		float const innerDeadZoneRadius = g_analogStickRangeRadius * (isLeftStick ? settings.m_leftStickInnerDeadzone : settings.m_rightStickInnerDeadzone);
		float const outerDeadZoneRadius = g_analogStickRangeRadius * (1.0f - (isLeftStick ? settings.m_leftStickOuterDeadzone : settings.m_rightStickOuterDeadzone));
		ImVec2 const analogStickCenter{ position.x + g_analogStickRangeRadius,position.y + g_analogStickRangeRadius };
		pDrawList->AddCircle(analogStickCenter, g_analogStickRangeRadius, g_controlFillColor, 40);
		pDrawList->AddCircleFilled(analogStickCenter, outerDeadZoneRadius, g_controlFillColor, 40);
		pDrawList->AddCircleFilled(analogStickCenter, innerDeadZoneRadius, 0xFF333333, 20);

		// Draw raw stick position
		ImVec2 stickOffset{ rawValue.x * g_analogStickRangeRadius, rawValue.y * g_analogStickRangeRadius };
		ImVec2 center{ analogStickCenter.x + stickOffset.x ,analogStickCenter.y + stickOffset.y };
		pDrawList->AddCircleFilled(center, g_analogStickPositionRadius, 0xFF0000FF, 6);

		// Draw filtered stick position
		glm::vec2 direction = (filteredValue.x + filteredValue.y != 0.f) ? glm::normalize(filteredValue) : glm::vec2{ 0,0 }; // fix NaN bug when trying to normalize filteredValue{0,0}
		glm::vec2 vec2StickOffest = (filteredValue * (outerDeadZoneRadius - innerDeadZoneRadius)) + (direction * innerDeadZoneRadius);
		stickOffset = { vec2StickOffest.x,vec2StickOffest.y };
		ImVec2 analogStickOffsetPosition{ analogStickCenter.x + stickOffset.x, analogStickCenter.y + stickOffset.y };
		pDrawList->AddCircleFilled(analogStickOffsetPosition, g_analogStickPositionRadius, 0xFF00FF00, 6);
	}

	void dae::Input::InputDebugImguiComponent::Initialize()
	{
		EventSubscribe(OnDeath);
	}

	void dae::Input::InputDebugImguiComponent::DrawUI([[maybe_unused]] UpdateContext& context)
	{
		DrawIsDeviceConnectedWindow();

		DrawDebugGamepadInputVisualisationWindow();
	}


	void dae::Input::InputDebugImguiComponent::DrawIsDeviceConnectedWindow()
	{
		ImGui::Begin("Input Debug Window");
		static auto& inputSystem = Input::InputSystem::GetInstance();

		static std::array<Input::InputDeviceGamepad*, XUSER_MAX_COUNT> gamepads
		{
			inputSystem.GetGamepadDevice(0),
			inputSystem.GetGamepadDevice(1),
			inputSystem.GetGamepadDevice(2),
			inputSystem.GetGamepadDevice(3)
		};


		for (size_t i = 0; i < XUSER_MAX_COUNT; i++)
		{
			ImGui::Text("Gamepad %d is", i);
			ImGui::SameLine();
			bool conntected = gamepads[i]->GetGamepadState().IsConnected();
			if (conntected)
			{
				ImGui::Text("connected.");

				if (gamepads[i]->GetGamepadState().IsHeldDown(XINPUT_GAMEPAD_A))
				{
					ImGui::SameLine();
					ImGui::Text("\tBUTTON A PRESSED.");
				}
			}
			else
			{
				ImGui::Text("not connected.");
			}
		}

		if(ImGui::Button("this is test button"))
		{
			EventManager::GetInstance().SendEvent(dae::OnDeathEvent{"Player1"});
		}
		ImGui::Text("%d", DeathCounter);

		ImGui::End();
	}

	void dae::Input::InputDebugImguiComponent::DrawDebugGamepadInputVisualisationWindow()
	{
		static auto& inputSystem = Input::InputSystem::GetInstance();

		static std::array<Input::InputDeviceGamepad*, XUSER_MAX_COUNT> gamepads
		{
			inputSystem.GetGamepadDevice(0),
			inputSystem.GetGamepadDevice(1),
			inputSystem.GetGamepadDevice(2),
			inputSystem.GetGamepadDevice(3)
		};

		for (size_t i = 0; i < XUSER_MAX_COUNT; i++)
		{
			if (!gamepads[i]->GetGamepadState().IsConnected()) continue;

			ImGui::SetNextWindowSize(ImVec2(300, 210));

			if (ImGui::Begin(std::format("Input Gamepad {}", (i + 1)).c_str(), nullptr, ImGuiWindowFlags_NoResize))
			{
				auto const& controllerState = gamepads[i]->GetGamepadState();

				ImDrawList* pDrawList = ImGui::GetWindowDrawList();
				ImVec2 const FirstRowTopLeft = ImGui::GetCursorScreenPos();
				ImVec2 const triggerButtonDimensions(g_buttonWidth, (g_analogStickRangeRadius * 2) + (g_buttonWidth * 2) + 8);

				glm::vec2 totalSize;
				ImVec2 drawPosition;

				// Left Shoulder and trigger buttons
				drawPosition = FirstRowTopLeft;
				DrawButton(pDrawList, drawPosition, g_shoulderDimensions, "LB", controllerState.IsHeldDown(XINPUT_GAMEPAD_LEFT_SHOULDER), g_controlOutlineColor, g_controlFillColor, true);
				drawPosition.y += g_buttonDimensions.y;
				DrawTriggerButton(pDrawList, drawPosition, triggerButtonDimensions, "LT", controllerState, true);

				// Left analog stick
				drawPosition = ImVec2(drawPosition.x + g_buttonDimensions.x + 9, FirstRowTopLeft.y);
				DrawAnalogStick(pDrawList, drawPosition, *gamepads[i], true);

				// Right analog stick
				drawPosition = ImVec2(drawPosition.x + 24 + g_analogStickRangeRadius * 2, FirstRowTopLeft.y);
				DrawAnalogStick(pDrawList, drawPosition, *gamepads[i], false);

				// Right Shoulder and trigger buttons
				drawPosition = ImVec2(drawPosition.x + g_analogStickRangeRadius * 2 + 9, FirstRowTopLeft.y);

				float rightShoulderOffset = g_shoulderDimensions.x - g_buttonDimensions.y;
				drawPosition.x -= rightShoulderOffset;
				DrawButton(pDrawList, drawPosition, g_shoulderDimensions, "RB", controllerState.IsHeldDown(XINPUT_GAMEPAD_RIGHT_SHOULDER), g_controlOutlineColor, g_controlFillColor, true);

				drawPosition.x += rightShoulderOffset;
				drawPosition.y += g_buttonDimensions.y;
				DrawTriggerButton(pDrawList, drawPosition, triggerButtonDimensions, "RT", controllerState, false);

				totalSize.x = (drawPosition.x + g_buttonWidth) - FirstRowTopLeft.x;
				totalSize.y = (g_analogStickRangeRadius * 2) + 14;

				//-------------------------------------------------------------------------

				glm::vec2 SecondRowTopLeft = glm::vec2(FirstRowTopLeft.x, FirstRowTopLeft.y + totalSize.y);

				// D-Pad
				const float multiply = 0.85f; // size of dpad buttons 
				const float dpadWidth = g_buttonDimensions.x * multiply; // dont touch
				const float dpadOffset = g_buttonWidth * 0.9f;
				float const upButtonTopLeft = SecondRowTopLeft.x + (g_buttonWidth * multiply + g_analogStickRangeRadius) - g_buttonWidth * multiply / 2;
				auto dpadButtonDimensions = g_buttonDimensions;
				drawPosition = ImVec2(upButtonTopLeft, SecondRowTopLeft.y - dpadOffset * (g_dpadStretchMultiplier-1));
				dpadButtonDimensions = { dpadWidth ,dpadWidth };
				dpadButtonDimensions.y *= g_dpadStretchMultiplier;
				DrawButton(pDrawList, drawPosition, dpadButtonDimensions, "U", controllerState.IsHeldDown(XINPUT_GAMEPAD_DPAD_UP), g_controlOutlineColor, g_controlFillColor, true);
				drawPosition = ImVec2(upButtonTopLeft - dpadOffset * g_dpadStretchMultiplier, SecondRowTopLeft.y + dpadOffset);
				dpadButtonDimensions = { dpadWidth ,dpadWidth };
				dpadButtonDimensions.x *= g_dpadStretchMultiplier;
				DrawButton(pDrawList, drawPosition, dpadButtonDimensions, "L", controllerState.IsHeldDown(XINPUT_GAMEPAD_DPAD_LEFT), g_controlOutlineColor, g_controlFillColor, true);
				drawPosition = ImVec2(upButtonTopLeft + dpadOffset, SecondRowTopLeft.y + dpadOffset);
				dpadButtonDimensions = { dpadWidth ,dpadWidth };
				dpadButtonDimensions.x *= g_dpadStretchMultiplier;
				DrawButton(pDrawList, drawPosition, dpadButtonDimensions, "R", controllerState.IsHeldDown(XINPUT_GAMEPAD_DPAD_RIGHT), g_controlOutlineColor, g_controlFillColor, true);
				drawPosition = ImVec2(upButtonTopLeft, SecondRowTopLeft.y + dpadOffset + dpadOffset);
				dpadButtonDimensions = { dpadWidth ,dpadWidth };
				dpadButtonDimensions.y *= g_dpadStretchMultiplier;
				DrawButton(pDrawList, drawPosition, dpadButtonDimensions, "D", controllerState.IsHeldDown(XINPUT_GAMEPAD_DPAD_DOWN), g_controlOutlineColor, g_controlFillColor, true);

				// Face Buttons
				float const topFaceButtonTopLeft = SecondRowTopLeft.x + ((g_buttonWidth + g_analogStickRangeRadius) - g_buttonWidth / 2) * 2 + 34 + (g_buttonWidth * 2);

				SecondRowTopLeft.y -= 6;

				drawPosition = ImVec2(topFaceButtonTopLeft, SecondRowTopLeft.y);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "Y", controllerState.IsHeldDown(XINPUT_GAMEPAD_Y), 0xFF00FFFF, 0xAA00AAAA);
				drawPosition = ImVec2(topFaceButtonTopLeft - g_buttonWidth, SecondRowTopLeft.y + g_buttonWidth);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "X", controllerState.IsHeldDown(XINPUT_GAMEPAD_X), 0xFFFF0000, 0xAAAA0000);
				drawPosition = ImVec2(topFaceButtonTopLeft + g_buttonWidth, SecondRowTopLeft.y + g_buttonWidth);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "B", controllerState.IsHeldDown(XINPUT_GAMEPAD_B), 0xFF0000FF, 0xAA0000AA);
				drawPosition = ImVec2(topFaceButtonTopLeft, SecondRowTopLeft.y + g_buttonWidth + g_buttonWidth);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "A", controllerState.IsHeldDown(XINPUT_GAMEPAD_A), 0xFF00FF00, 0xAA00AA00);

				// System Buttons
				drawPosition = ImVec2(SecondRowTopLeft.x + g_buttonWidth + g_analogStickRangeRadius * 2, SecondRowTopLeft.y + 10);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "S0", controllerState.IsHeldDown(XINPUT_GAMEPAD_BACK));
				drawPosition = ImVec2(drawPosition.x + g_buttonWidth + 4, drawPosition.y);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "S1", controllerState.IsHeldDown(XINPUT_GAMEPAD_START));

				// Stick Buttons
				drawPosition = ImVec2(SecondRowTopLeft.x + g_buttonWidth + g_analogStickRangeRadius * 2, drawPosition.y + g_buttonWidth + 4);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "LS", controllerState.IsHeldDown(XINPUT_GAMEPAD_LEFT_THUMB));
				drawPosition = ImVec2(drawPosition.x + g_buttonWidth + 4, drawPosition.y);
				DrawButton(pDrawList, drawPosition, g_buttonDimensions, "RS", controllerState.IsHeldDown(XINPUT_GAMEPAD_RIGHT_THUMB));

				totalSize.x = (drawPosition.x + g_buttonWidth) - FirstRowTopLeft.x;
				totalSize.y = triggerButtonDimensions.y + g_buttonWidth + 4;
			}
			ImGui::End();

		}
	}

	void InputDebugImguiComponent::Clone(GameObject* clone)
	{
		if (CanBeCloned() == false) return;
		clone->AddComponent<InputDebugImguiComponent>(GetComponentTag());
	}

#pragma endregion
}
