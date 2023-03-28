#pragma once
#include "Singleton.h"
#include "Timers.h"
#include "InputDeviceKeyboardMouse.h"
#include "InputDeviceGamepad.h"
#include <vector>
#include <memory>
#include "CommandHandler.h"
//-------------------------------------------------------------------------

namespace dae::Input
{
    class InputState;

    // input system, manages all hardware devices and updates their state

    class InputSystem : public Singleton<InputSystem>  // This should be called InputManager but NO
    {

        friend class InputDebugImguiComponent;

    public:

        static InputStateMouse const s_emptyMouseState;
        static InputStateKeyboard const s_emptyKeyboardState;
        static InputStateGamepad const s_emptyGamepadState;

    public:

        bool Initialize();
        void Shutdown();
        bool ProcessInput(const UpdateContext& context);

        // Keyboard & Mouse
        //-------------------------------------------------------------------------

        inline bool HasConnectedKeyboardAndMouse() { return GetKeyboardMouseDevice() != nullptr; }

        InputDeviceKeyboardMouse* GetKeyboardMouseDevice() const;
        InputStateKeyboard const* GetKeyboardState() const;
        InputStateMouse const* GetMouseState() const;

        // Gamepads
        //-------------------------------------------------------------------------

        uint32_t GetNumConnectedGamepads() const;

        InputStateGamepad const* GetGamepadState(uint8_t GamepadIdx = 0) const;
        InputDeviceGamepad* GetGamepadDevice(uint8_t GamepadIdx = 0) const;

        // CommandHandler
        //-------------------------------------------------------------------------
        CommandHandler* GetCommandHandler() const { return m_CommandHandler.get(); }

    private:
        friend class Singleton<InputSystem>;
        InputSystem() = default;

    private:

        std::vector<std::unique_ptr<InputDevice>>  m_inputDevices;
        std::unique_ptr<CommandHandler> m_CommandHandler;
    };
}