#pragma once
#include "glm\glm.hpp"
#include "Time.h"

namespace dae::Input
{
    enum class MouseButton
    {
        Left = 0,
        Right,
        Middle,
        Back,
        Forward,
        Button5,
        Button6,
        Button7,
        Button8,
        Button9,
    };

    //-------------------------------------------------------------------------

    class InputStateMouse
    {
        friend class InputDeviceKeyboardMouse;

    public:

        enum class WheelType
        {
            Vertical,
            Horizontal
        };

    public:


        inline glm::vec2 GetMovementDelta() const { return m_movementDelta; }
        inline int16_t GetWheelDelta(WheelType wheelType = WheelType::Vertical) const { return (wheelType == WheelType::Vertical) ? m_verticalWheelDelta : m_horizontalWheelDelta; }
        inline glm::ivec2 GetCursorPosition() const { return m_position; }

        // Was the button just pressed (i.e. went from up to down this frame)
        bool WasPressed(MouseButton buttonID) const { (buttonID); return 0; }

        // Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
        bool WasReleased(MouseButton buttonID) const { (buttonID); return 0; }

        // Is the button being held down?
       bool IsHeldDown(MouseButton buttonID) const { (buttonID); return 0; }

    private:
        bool ProcessInput(Seconds deltaTime) { (deltaTime); return true; };

    private:

        glm::vec2                                 m_movementDelta;
        glm::ivec2                                m_position;
        int16_t                                   m_verticalWheelDelta = 0;
        int16_t                                   m_horizontalWheelDelta = 0;
    };
}