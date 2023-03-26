#include "InputDeviceGamepad.h"
#include <assert.h>
#include "glm\glm.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"
//-------------------------------------------------------------------------

namespace dae::Input
{
	static float const g_maxThumbstickRange = 32767.0f;
	static float const g_maxTriggerRange = 255.0f;

	void InputDeviceGamepad::Initialize()
	{
		XINPUT_STATE gamepadState;
		DWORD result = XInputGetState(m_GamepadIndex, &gamepadState);
		m_GamepadState.m_IsConnected = (result == ERROR_SUCCESS);

		m_settings.m_leftStickInnerDeadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / g_maxThumbstickRange;
		m_settings.m_leftStickOuterDeadzone = 0.0f;
		m_settings.m_leftTriggerThreshold = XINPUT_GAMEPAD_TRIGGER_THRESHOLD / g_maxTriggerRange;

		m_settings.m_rightStickInnerDeadzone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / g_maxThumbstickRange;
		m_settings.m_rightStickOuterDeadzone = 0.0f;
		m_settings.m_rightTriggerThreshold = XINPUT_GAMEPAD_TRIGGER_THRESHOLD / g_maxTriggerRange;
	}

	void InputDeviceGamepad::Shutdown()
	{
		m_GamepadState.m_IsConnected = false;
	}

	bool InputDeviceGamepad::ProcessInput(Seconds deltaTime)
	{
		XINPUT_STATE gamepadState;
		DWORD result = XInputGetState(m_GamepadIndex, &gamepadState);

		m_GamepadState.m_IsConnected = (result == ERROR_SUCCESS);

		if (m_GamepadState.m_IsConnected)
		{
			SetTriggerValues(gamepadState.Gamepad.bLeftTrigger / g_maxTriggerRange, gamepadState.Gamepad.bRightTrigger / g_maxTriggerRange);
			SetAnalogStickValues(glm::vec2(gamepadState.Gamepad.sThumbLX / g_maxThumbstickRange, gamepadState.Gamepad.sThumbLY / g_maxThumbstickRange), glm::vec2(gamepadState.Gamepad.sThumbRX / g_maxThumbstickRange, gamepadState.Gamepad.sThumbRY / g_maxThumbstickRange));

			m_GamepadState.ProcessInput(deltaTime, gamepadState);
		}

		return true;
	}

	void InputDeviceGamepad::SetTriggerValues(float leftRawValue, float rightRawValue)
	{
		auto calculateFilteredTriggerValue = [](float rawValue, float threshold)
		{
			assert(threshold >= 0 && threshold <= 1.0f);

			float filteredValue = 0.0f;
			if (rawValue > threshold)
			{
				filteredValue = (rawValue - threshold) / (1.0f - threshold);
			}

			return filteredValue;
		};

		//-------------------------------------------------------------------------

		m_GamepadState.m_triggerRaw[InputStateGamepad::Direction::Left] = leftRawValue;
		m_GamepadState.m_triggerRaw[InputStateGamepad::Direction::Right] = rightRawValue;
		m_GamepadState.m_triggerFiltered[InputStateGamepad::Direction::Left] = calculateFilteredTriggerValue(leftRawValue, m_settings.m_leftTriggerThreshold);
		m_GamepadState.m_triggerFiltered[InputStateGamepad::Direction::Right] = calculateFilteredTriggerValue(rightRawValue, m_settings.m_rightTriggerThreshold);
	}

	void InputDeviceGamepad::SetAnalogStickValues(glm::vec2 const& leftRawValue, glm::vec2 const& rightRawValue)
	{
		auto calculateRawValue = [](const glm::vec2 rawValue, bool bInvertY)
		{
			float const normalizedX = glm::clamp(rawValue.x, -1.0f, 1.0f);
			float const normalizedY = glm::clamp(rawValue.y, -1.0f, 1.0f);
			return glm::vec2(normalizedX, bInvertY ? -normalizedY : normalizedY);
		};

		m_GamepadState.m_analogInputRaw[InputStateGamepad::Direction::Left] = calculateRawValue(leftRawValue, m_settings.m_leftStickInvertY);
		m_GamepadState.m_analogInputRaw[InputStateGamepad::Direction::Right] = calculateRawValue(rightRawValue, m_settings.m_rightStickInvertY);

		auto calculateFilteredValue = [](const glm::vec2& rawValue, const float innerDeadzoneRange, const float outerDeadzoneRange) -> glm::vec2
		{
			assert(innerDeadzoneRange >= 0 && innerDeadzoneRange <= 1.0f && outerDeadzoneRange >= 0 && outerDeadzoneRange <= 1.0f);

			glm::vec2 filteredValue;

			// Get the direction and magnitude
			glm::vec2 direction{ rawValue };
			float magnitude = glm::length(direction);
			direction = glm::normalize(direction);

			// Apply dead zones
			if (magnitude > innerDeadzoneRange)
			{
				const float remainingRange = (1.0f - outerDeadzoneRange - innerDeadzoneRange);
				const float magnitudeWithoutDeadzones = ((magnitude - innerDeadzoneRange) / remainingRange);
				const float clampedMagnitude = (((1.0f) < (magnitudeWithoutDeadzones)) ? (1.0f) : (magnitudeWithoutDeadzones)); // THIS IS LITERALLY COPY PASTE FROM GLM::MIN BECAUSE YOU DONT WANT TO WORK WHEN CALLED DIRECTLY
				filteredValue = (direction * clampedMagnitude);
			}
			else // Set the value to zero
			{
				filteredValue = glm::vec2{};
			}

			return filteredValue;
		};

		m_GamepadState.m_analogInputFiltered[InputStateGamepad::Direction::Left] = calculateFilteredValue(m_GamepadState.m_analogInputRaw[InputStateGamepad::Direction::Left], m_settings.m_leftStickInnerDeadzone, m_settings.m_leftStickOuterDeadzone);
		m_GamepadState.m_analogInputFiltered[InputStateGamepad::Direction::Right] = calculateFilteredValue(m_GamepadState.m_analogInputRaw[InputStateGamepad::Direction::Right], m_settings.m_rightStickInnerDeadzone, m_settings.m_rightStickOuterDeadzone);
	}
}