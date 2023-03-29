#pragma once
#include "InputDevice.h"
#include "InputStateKeyboard.h"

namespace dae
{
    namespace Input
    {
        class InputDeviceKeyboard final : public InputDevice // ALL SDL GETS HANDLED IN THIS CLASS
        {
        public:

            InputDeviceKeyboard() = default;

            inline const InputStateKeyboard& GetKeyboardState() const { return m_keyboardState; }

        private:

            virtual DeviceCategory GetDeviceCategory() const override { return DeviceCategory::KeyboardMouse; }

            virtual void Initialize() override {};
            virtual void Shutdown() override {};

            virtual bool ProcessInput(Seconds deltaTime) override 
            {
                
            };

        private:

            InputStateKeyboard                                      m_keyboardState;
        };
    }
}// Mutable,