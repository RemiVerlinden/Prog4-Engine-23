#pragma once
#include "InputDevice.h"
#include "InputStateMouse.h"

namespace dae
{
    namespace Input
    {
        class InputDeviceKeyboard final : public InputDevice // ALL SDL GETS HANDLED IN THIS CLASS
        {
        public:

            InputDeviceKeyboard() = default;

            inline const InputStateMouse& GetKeyboardState() const { return m_MouseState; }

        private:

            virtual DeviceCategory GetDeviceCategory() const override { return DeviceCategory::KeyboardMouse; }

            virtual void Initialize() override {};
            virtual void Shutdown() override {};

            virtual bool ProcessInput(Seconds deltaTime) override
            {

            };

        private:

            InputStateMouse                                      m_MouseState;
        };
    }
}// Mutable,