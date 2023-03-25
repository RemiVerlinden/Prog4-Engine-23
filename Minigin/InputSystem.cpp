#include "InputSystem.h"

//-------------------------------------------------------------------------

namespace dae::Input
{
    InputStateMouse const InputSystem::s_emptyMouseState = InputStateMouse();
    InputStateKeyboard const InputSystem::s_emptyKeyboardState = InputStateKeyboard();
    InputStateGamepad const InputSystem::s_emptyGamepadState = InputStateGamepad();

    //-------------------------------------------------------------------------

    bool InputSystem::Initialize()
    {
        // Create a keyboard and mouse device
        m_inputDevices.push_back(std::move(std::make_unique<InputDeviceKeyboardMouse>(InputDeviceKeyboardMouse())));

        // Create Gamepads
        //-------------------------------------------------------------------------

        for (uint8_t GamepadIdx = 0; GamepadIdx < XUSER_MAX_COUNT; ++GamepadIdx)
        {
             m_inputDevices.push_back(std::move(std::make_unique<InputDeviceGamepad>(InputDeviceGamepad(GamepadIdx))));
        }

        //-------------------------------------------------------------------------

        for (auto& pDevice : m_inputDevices)
        {
            pDevice->Initialize();
        }

        return true;
    }

    void InputSystem::Shutdown()
    {
        for (auto& pInputDevice : m_inputDevices)
        {
            pInputDevice->Shutdown();
        }
    }

    bool InputSystem::ProcessInput(Seconds deltaTime)
    {
        bool doContinue = true;
        for (auto& pInputDevice : m_inputDevices)
        {
            doContinue = static_cast<bool>(pInputDevice->ProcessInput(deltaTime) | doContinue);
        }
        return doContinue;
    }

    //-------------------------------------------------------------------------

    InputDeviceKeyboardMouse* InputSystem::GetKeyboardMouseDevice() const
    {
        for (auto& pDevice : m_inputDevices)
        {
            if (pDevice->GetDeviceCategory() == DeviceCategory::KeyboardMouse)
            {
                return static_cast<InputDeviceKeyboardMouse*>(pDevice.get());
            }
        }

        return nullptr;
    }

    //-------------------------------------------------------------------------

    uint32_t InputSystem::GetNumConnectedGamepads() const
    {
        uint32_t numGamepads = 0;
        for (auto& pDevice : m_inputDevices)
        {
            if (pDevice->GetDeviceCategory() == DeviceCategory::Gamepad)
            {
                auto pGamepad = static_cast<InputDeviceGamepad*>(pDevice.get());
                if (pGamepad->IsConnected())
                {
                    ++numGamepads;
                }
            }
        }

        return numGamepads;
    }

    InputDeviceGamepad* InputSystem::GetGamepadDevice(uint8_t GamepadIdx) const
    {
        for (auto& pDevice : m_inputDevices)
        {
            if (pDevice->GetDeviceCategory() == DeviceCategory::Gamepad)
            {
                auto pGamepad = static_cast<InputDeviceGamepad*>(pDevice.get());

                if (!pGamepad->IsConnected()) continue;
                if (pGamepad->GetGamepadIndex() != GamepadIdx) continue;

                return pGamepad;
            }
        }

        return nullptr;
    }
}