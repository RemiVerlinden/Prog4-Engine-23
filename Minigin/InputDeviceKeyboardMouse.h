#pragma once
#include "InputDevice.h"
#include "InputStateKeyboard.h"
#include "InputStateMouse.h"
namespace dae
{
	namespace Input
	{
		class InputDeviceKeyboardMouse final : public InputDevice // ALL SDL GETS HANDLED IN THIS CLASS
		{
        public:

            InputDeviceKeyboardMouse() = default;

            inline const InputStateMouse& GetMouseState() const { return m_mouseState; }
            inline const InputStateKeyboard& GetKeyboardState() const { return m_keyboardState; }

        private:

            virtual DeviceCategory GetDeviceCategory() const override { return DeviceCategory::KeyboardMouse; }

            virtual void Initialize() override;
            virtual void Shutdown() override;

            virtual bool ProcessInput(Seconds deltaTime) override;

        private:

            InputStateMouse                                         m_mouseState;
            InputStateKeyboard                                      m_keyboardState;
		};
	}
}// Mutable, CRTF, structured bindinng, std::tuple