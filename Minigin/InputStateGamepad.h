#pragma once

#include "InputState.h"
#include "glm\glm.hpp"

//-------------------------------------------------------------------------

namespace dae::Input
{
	enum Direction : int
	{
		Left = 0,
		Right = 1,
		size = 2
	};


	class InputStateGamepad final : public InputState
	{
		friend class InputDeviceGamepad;

	public:

		InputStateGamepad();
		~InputStateGamepad();

		// Get the filtered value of the left analog stick once the deadzone has been applied
		glm::vec2 GetLeftAnalogStickValue() const;

		// Get the raw pointer to the stick, used in the moveCommand class
		const glm::vec2* GetAnalogStickFilteredPtr(bool rightStick) const;

		// Get the filtered value of the right analog stick once the deadzone has been applied
		glm::vec2 GetRightAnalogStickValue() const;

		// Get the filtered value of the left trigger once the trigger thresholds has been applied
		float GetLeftTriggerValue() const;

		// Get the filtered value of the right trigger once the trigger thresholds has been applied
		float GetRightTriggerValue() const;

		// Get the raw unfiltered value of the left analog stick
		glm::vec2 GetLeftAnalogStickRawValue() const;

		// Get the raw unfiltered value of the right analog stick
		glm::vec2 GetRightAnalogStickRawValue() const;

		// Get the raw unfiltered value of the left trigger
		float GetLeftTriggerRawValue() const;

		// Get the raw unfiltered value of the right trigger
		float GetRightTriggerRawValue() const;

		//-------------------------------------------------------------------------

		// Was the button just pressed (i.e. went from up to down this frame)
		virtual bool WasPressed(deviceButton button) const override;

		// Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
		virtual bool WasReleased(deviceButton button) const override;

		// Is the button being held down?
		virtual bool IsHeldDown(deviceButton button) const override;

		bool IsHeldDown(int button) const;

		bool IsConnected() const;
		void SetConnection(bool connected) const;

	private:

		int GetGamepadButton(deviceButton button) const;

	private:
		class InputStateGamepadImpl;

		InputStateGamepadImpl* m_pImpl;
	};

}