#include "InputDeviceGamepad.h"
#include <assert.h>
#include <algorithm> // include the algorithm header for std::min
#include "glm\glm.hpp"

#ifndef MY_MIN
#define MY_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"
//-------------------------------------------------------------------------

namespace dae::Input
{
	void InputDeviceGamepad::Initialize()
	{
	}
	void InputDeviceGamepad::Shutdown()
	{
	}
	bool InputDeviceGamepad::ProcessInput(Seconds deltaTime)
	{
		XINPUT_STATE gamepadState;
		//ZeroMemory(&gamepadState, sizeof(XINPUT_STATE));
		DWORD result = XInputGetState(m_GamepadIndex, &gamepadState);

		switch (result)
		{
		case ERROR_SUCCESS:
		{
			m_GamepadState.ProcessInput(deltaTime, gamepadState);
			m_IsConnected = true;
			break;
		}

		case ERROR_DEVICE_NOT_CONNECTED:
		default:
			m_IsConnected = false;
			break;
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
			glm::vec2 vDirection{ rawValue };
			glm::length_t magnitude;
			magnitude = vDirection.length();

			// Apply dead zones
			if (magnitude > innerDeadzoneRange)
			{
				const float remainingRange = (1.0f - outerDeadzoneRange - innerDeadzoneRange);
				const float magnitudeWithoutDeadzones = ((magnitude - innerDeadzoneRange) / remainingRange);
				const float clampedMagnitude = (((1.0f) < (magnitudeWithoutDeadzones)) ? (1.0f) : (magnitudeWithoutDeadzones)); // THIS IS LITERALLY COPY PASTE FROM GLM::MIN BECAUSE YOU DONT WANT TO WORK WHEN CALLED DIRECTLY
				filteredValue = (vDirection * clampedMagnitude);
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