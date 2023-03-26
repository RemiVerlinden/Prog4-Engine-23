#pragma once
#include "glm\glm.hpp"
#include "Time.h"
#include <array>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Xinput.h"
//-------------------------------------------------------------------------

namespace dae::Input
{
	enum class GamepadButton : uint16_t
	{
		DPadUp = 0,
		DPadDown,
		DPadLeft,
		DPadRight,
		ThumbstickLeft,
		ThumbstickRight,
		ShoulderLeft,
		ShoulderRight,
		System0,
		System1,
		FaceButtonDown,
		FaceButtonRight,
		FaceButtonLeft,
		FaceButtonUp,
	};

	//-------------------------------------------------------------------------

	class InputStateGamepad
	{
		friend class InputDeviceGamepad;

		enum Direction : int
		{
			Left = 0,
			Right = 1,
			size = 2
		};

		struct GamepadButtonStates
		{
			bool connected = false;
			XINPUT_GAMEPAD previousButtonState{};
			XINPUT_GAMEPAD currentButtonState{};

			WORD buttonsPressedThisFrame{};
			WORD buttonsReleasedThisFrame{};

			XINPUT_BATTERY_INFORMATION batteryInformation{};
		};

	public:

		InputStateGamepad() = default;

		// Get the filtered value of the left analog stick once the deadzone has been applied
		inline glm::vec2 GetLeftAnalogStickValue() const { return m_analogInputFiltered[Left]; }

		// Get the filtered value of the right analog stick once the deadzone has been applied
		inline glm::vec2 GetRightAnalogStickValue() const { return m_analogInputFiltered[Right]; }

		// Get the filtered value of the left trigger once the trigger thresholds has been applied
		inline float GetLeftTriggerValue() const { return m_triggerFiltered[Left]; }

		// Get the filtered value of the right trigger once the trigger thresholds has been applied
		inline float GetRightTriggerValue() const { return m_triggerFiltered[Right]; }

		// Get the raw unfiltered value of the left analog stick
		inline glm::vec2 GetLeftAnalogStickRawValue() const { return m_analogInputRaw[Left]; }

		// Get the raw unfiltered value of the right analog stick
		inline glm::vec2 GetRightAnalogStickRawValue() const { return m_analogInputRaw[Right]; }

		// Get the raw unfiltered value of the left trigger
		inline float GetLeftTriggerRawValue() const { return m_triggerRaw[Left]; }

		// Get the raw unfiltered value of the right trigger
		inline float GetRightTriggerRawValue() const { return m_triggerRaw[Right]; }

		//-------------------------------------------------------------------------

		// Was the button just pressed (i.e. went from up to down this frame)
		bool WasPressed(uint64_t button) const 
		{
			return (m_ButtonStates.buttonsPressedThisFrame & button);
		}

		// Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
		bool WasReleased(uint64_t button) const
		{
			return (m_ButtonStates.buttonsReleasedThisFrame & button);
		}

		// Is the button being held down?
		bool IsHeldDown(uint64_t button) const
		{ 
			return (m_ButtonStates.currentButtonState.wButtons & button);
		}

		inline bool IsConnected() const { return m_IsConnected; }

	private:
		bool ProcessInput(Seconds deltaTime, XINPUT_STATE& gamepadState) 
		{
			std::swap(m_ButtonStates.previousButtonState, m_ButtonStates.currentButtonState);
			m_ButtonStates.currentButtonState = gamepadState.Gamepad;

			WORD buttonChanges = m_ButtonStates.currentButtonState.wButtons ^ m_ButtonStates.previousButtonState.wButtons;
			m_ButtonStates.buttonsPressedThisFrame = buttonChanges & m_ButtonStates.currentButtonState.wButtons;
			m_ButtonStates.buttonsReleasedThisFrame = buttonChanges & (~m_ButtonStates.currentButtonState.wButtons);

			(deltaTime);
			return true; 
		};

	private:

		GamepadButtonStates								m_ButtonStates;
		std::array<glm::vec2, (int)Direction::size>		m_analogInputRaw = { glm::vec2{}, glm::vec2{} };
		std::array<glm::vec2, Direction::size>			m_analogInputFiltered = { glm::vec2{}, glm::vec2{} };
		std::array<float, Direction::size>				m_triggerRaw = { 0.0f, 0.0f };
		std::array<float, Direction::size>				m_triggerFiltered = { 0.0f, 0.0f };
		bool                                        m_IsConnected = false;
	};
}