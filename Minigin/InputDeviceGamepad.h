#pragma once
#include "InputDevice.h"
#include "InputStateGamepad.h"

//-------------------------------------------------------------------------

namespace dae
{
	namespace Input
	{

		//-------------------------------------------------------------------------

		struct GamepadSettings
		{

		public:

			float     m_leftStickInnerDeadzone = 0.0f;
			float     m_leftStickOuterDeadzone = 0.0f;
			float     m_leftTriggerThreshold = 0.0f;

			float     m_rightStickInnerDeadzone = 0.0f;
			float     m_rightStickOuterDeadzone = 0.0f;
			float     m_rightTriggerThreshold = 0.0f;

			bool    m_leftStickInvertY = false;
			bool    m_rightStickInvertY = false;
		};
		//-------------------------------------------------------------------------

		class InputDeviceGamepad final : public InputDevice // ALL XINPUT IS HANDLED HERE AND IN INPUTSTATEGAMEPAD.H
		{
		public:

			InputDeviceGamepad(uint8_t gamepadIndex);
			~InputDeviceGamepad();

			const GamepadSettings& GetSettings() const;
			const InputStateGamepad& GetGamepadState() const;

			bool IsConnected() const;
			void SetConnection(bool connection);
			uint8_t GetGamepadIndex()const;

			virtual void Initialize() override;
			virtual void Shutdown() override;

		protected:

			virtual bool ProcessInput(Seconds deltaTime) override;

			void SetTriggerValues(float leftRawValue, float rightRawValue);
			void SetAnalogStickValues(const glm::vec2& leftRawValue, const glm::vec2& rightRawValue);

		private:

			virtual DeviceCategory GetDeviceCategory() const override;


			class InputDeviceGamepadImpl;

			InputDeviceGamepadImpl* m_pImpl;
		};
	}
}
