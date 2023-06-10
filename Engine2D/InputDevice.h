#pragma once
#include "EngineTime.h"
#include "InputState.h"
//-------------------------------------------------------------------------

namespace engine
{
    namespace Input
    {

        enum class DeviceCategory // might expand in coming weeks (probably not)
        {
            Keyboard,
            Mouse,
            Gamepad
        };

        class InputDevice
        {

        public:
            InputDevice() : m_ID(s_NextId++) {}

            virtual ~InputDevice() = default;
            virtual void Initialize() = 0;
            virtual void Shutdown() = 0;

            // Get the category for this device (Gamepad/mice), this is necessary since we may have multiple Gamepad devices
            virtual DeviceCategory GetDeviceCategory() const = 0;
            virtual const InputState& GetDeviceInputState() const = 0;

            // Called at the start of the frame to update the current device state
            virtual void ProcessInput(Seconds deltaTime) = 0;

            int GetID() { return m_ID; }

        private:
            const int m_ID;
            static inline int s_NextId = 0;
        };

    }
}