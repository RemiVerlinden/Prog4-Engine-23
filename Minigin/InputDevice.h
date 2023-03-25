#pragma once
#include "Time.h"

//-------------------------------------------------------------------------

namespace dae
{
    namespace Input
    {

        enum class DeviceCategory
        {
            KeyboardMouse,
            Gamepad
        };

        class InputDevice
        {

        public:

            virtual ~InputDevice() = default;
            virtual void Initialize() = 0;
            virtual void Shutdown() = 0;

            // Get the category for this device (Gamepad/mice), this is necessary since we may have multiple Gamepad device types (e.g. XBox, PS, 3rdParty)
            virtual DeviceCategory GetDeviceCategory() const = 0;

            // Called at the start of the frame to update the current device state
            virtual bool ProcessInput(Seconds deltaTime) = 0;
        };
    }
}