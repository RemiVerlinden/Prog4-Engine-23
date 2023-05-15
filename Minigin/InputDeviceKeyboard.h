#pragma once
#include "InputDevice.h"
#include "InputStateKeyboard.h"
#include "SDLEventQueue.h"

namespace dae
{
    namespace Input
    {
        class InputDeviceKeyboard final : public InputDevice // ALL SDL GETS HANDLED IN THIS CLASS
        {
        public:

            InputDeviceKeyboard(SDLEventQueue& eventQueue) :m_EventQueue(eventQueue) {};

            inline const InputStateKeyboard& GetKeyboardState() const { return m_KeyboardState; }
            virtual const InputState& GetDeviceInputState() const override { return m_KeyboardState; }

        private:

            virtual DeviceCategory GetDeviceCategory() const override { return DeviceCategory::Keyboard; }

            virtual void Initialize() override {};
            virtual void Shutdown() override {};

            virtual void ProcessInput(Seconds deltaTime) override 
            {
                for (const SDL_Event& event : m_EventQueue.GetEvents())
                {
                    switch (event.type)
                    {
                        case SDL_KEYDOWN:
                        {
							break;
						}
                        default:
							break;
                    }
                }
                m_KeyboardState.ProcessInput(deltaTime);
            };

        private:
            SDLEventQueue&                                          m_EventQueue;
            InputStateKeyboard                                      m_KeyboardState;
        };
    }
}// Mutable,