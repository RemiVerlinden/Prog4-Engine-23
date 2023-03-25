#pragma once
#include "InputDevice.h"
#include "InputStateGamepad.h"

namespace dae
{
	namespace Input
	{
		class InputDeviceGamepad final : public InputDevice
		{
		public:

			struct Settings
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

	public:

			InputDeviceGamepad(uint8_t GamepadIndex) : m_GamepadIndex(GamepadIndex) {}

            inline Settings const& GetSettings() const { return m_settings; }
            inline InputStateGamepad const& GetGamepadState() const { return m_GamepadState; }

            inline bool IsConnected() const { return m_IsConnected; }
			inline uint8_t GetGamepadIndex()const { return m_GamepadIndex; }

			virtual void Initialize() override;
			virtual void Shutdown() override;

        protected:

            virtual bool ProcessInput(Seconds deltaTime) override;

            void SetTriggerValues(float leftRawValue, float rightRawValue);
            void SetAnalogStickValues(const glm::vec2& leftRawValue, const glm::vec2& rightRawValue);

        private:

            virtual DeviceCategory GetDeviceCategory() const override final { return DeviceCategory::Gamepad; }

        protected:

            Settings                                    m_settings;
            InputStateGamepad							m_GamepadState;
            uint8_t                                     m_GamepadIndex;
            bool                                        m_IsConnected = false;
        };
    }
}
