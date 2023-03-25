#include "InputDeviceKeyboardMouse.h"

//-------------------------------------------------------------------------

namespace dae
{
    namespace Input
    {
        void InputDeviceKeyboardMouse::Initialize()
        {
        }
        void InputDeviceKeyboardMouse::Shutdown()
        {
        }
        bool InputDeviceKeyboardMouse::ProcessInput(Seconds deltaTime)
        {
            bool doContinue = true;
            doContinue = static_cast<bool>(m_mouseState.ProcessInput(deltaTime) | doContinue);
            doContinue = static_cast<bool>(m_keyboardState.ProcessInput(deltaTime) | doContinue);
            return doContinue;
        }
    }
}