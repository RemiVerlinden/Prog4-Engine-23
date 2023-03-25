//#pragma once
//#include "Time.h"
//#include <array>
//
////-------------------------------------------------------------------------
//
//namespace dae::Input
//{
//    class Button
//    {
//        template<uint32_t> friend class ButtonStates;
//
//        enum class State : uint8_t
//        {
//            None = 0,
//            Pressed,
//            Held,
//            Released,
//        };
//
//
//    public:
//
//        bool IsPressed() const { return m_state == State::Pressed; }
//        bool IsReleased() const { return m_state == State::Released; }
//        bool IsHeldDown() const { return m_state == State::Held; }
//
//        Seconds GetTimeHeld() const
//        {
//            return m_holdTime;
//        }
//
//        void Update(Seconds deltaTime)
//        {
//            if (m_state == Button::State::Held)
//            {
//                m_holdTime += deltaTime;
//            }
//        }
//
//    private:
//
//        // Sets the state of the button and flags that the button state has changed
//        void SetState(State inState)
//        {
//            m_state = inState;
//
//            if (inState != State::Held)
//            {
//                m_holdTime = 0.0f;
//            }
//        }
//
//    private:
//
//        Seconds             m_holdTime;
//        State               m_state = State::None;
//    };
//
//    //-------------------------------------------------------------------------
//
//    class XInputButtons
//    {
//
//    public:
//
//        inline bool WasPressed(uint32_t buttonIdx) const
//        {
//            static_assert(buttonIdx < numButtons);
//            return (m_buttons[buttonIdx].WasPressed());
//        }
//
//        inline bool WasReleased(uint32_t buttonIdx, Seconds* pHeldDownDuration = nullptr) const
//        {
//            static_assert(buttonIdx < numButtons);
//            if (m_buttons[buttonIdx].WasReleased())
//            {
//                if (pHeldDownDuration != nullptr)
//                {
//                    *pHeldDownDuration = m_buttons[buttonIdx].m_holdTime;
//                }
//
//                return true;
//            }
//
//            return false;
//        }
//
//        inline bool IsHeldDown(uint32_t buttonIdx, Seconds* pHeldDownDuration = nullptr) const
//        {
//            if (pHeldDownDuration != nullptr)
//            {
//                *pHeldDownDuration = GetHeldDuration(buttonIdx);
//            }
//
//            return WasPressed(buttonIdx) || m_buttons[buttonIdx].IsHeldDown();
//        }
//
//        inline Seconds GetHeldDuration(uint32_t buttonIdx) const
//        {
//            static_assert(buttonIdx < numButtons);
//            return m_buttons[buttonIdx].IsHeldDown() ? m_buttons[buttonIdx].GetTimeHeld() : Seconds(0.0f);
//        }
//
//    protected:
//
//        inline void ClearButtonState()
//        {
//            ZeroMemory(m_buttons.data(), m_buttons.size() * sizeof(Button));
//        }
//
//        inline void Update(Seconds deltaTime)
//        {
//            for (auto i = 0; i < numButtons; i++)
//            {
//                m_buttons[i].Update(deltaTime);
//            }
//        }
//
//        // Called when we detect a pressed event for a button
//        inline void Press(uint32_t buttonIdx)
//        {
//            m_buttons[buttonIdx].SetState(Button::State::Pressed);
//        }
//
//        // Called when we detect a released event for a button
//        inline void Release(uint32_t buttonIdx)
//        {
//            m_buttons[buttonIdx].SetState(Button::State::Released);
//        }
//
//    private:
//
//        std::array<Button, numButtons>   m_buttons;
//    };
//}