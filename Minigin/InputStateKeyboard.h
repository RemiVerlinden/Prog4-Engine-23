#pragma once
#include "glm\glm.hpp"
#include "Time.h"
#include <SDL.h>
#include "ImguiWrapper.h"
#include "SceneManager.h"
#include <iostream>
#include <array>

namespace dae::Input
{
	enum class KeyboardMouseButton : uint16_t
	{
		Key_A = 0,
		Key_B,
		Key_C,
		Key_D,
		Key_E,
		Key_F,
		Key_G,
		Key_H,
		Key_I,
		Key_J,
		Key_K,
		Key_L,
		Key_M,
		Key_N,
		Key_O,
		Key_P,
		Key_Q,
		Key_R,
		Key_S,
		Key_T,
		Key_U,
		Key_V,
		Key_W,
		Key_X,
		Key_Y,
		Key_Z,
		Key_0,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Key_Comma,
		Key_Period,
		Key_ForwardSlash,
		Key_SemiColon,
		Key_Quote,
		Key_LBracket,
		Key_RBracket,
		Key_BackSlash,
		Key_Underscore,
		Key_Minus = Key_Underscore,
		Key_Equals,
		Key_Plus = Key_Equals,
		Key_Backspace,
		Key_Tilde,
		Key_Tab,
		Key_CapsLock,
		Key_Enter,
		Key_Escape,
		Key_Space,
		Key_Left,
		Key_Up,
		Key_Right,
		Key_Down,
		Key_NumLock,
		Key_Numpad0,
		Key_Numpad1,
		Key_Numpad2,
		Key_Numpad3,
		Key_Numpad4,
		Key_Numpad5,
		Key_Numpad6,
		Key_Numpad7,
		Key_Numpad8,
		Key_Numpad9,
		Key_NumpadEnter,
		Key_NumpadMultiply,
		Key_NumpadPlus,
		Key_NumpadMinus,
		Key_NumpadPeriod,
		Key_NumpadDivide,
		Key_F1,
		Key_F2,
		Key_F3,
		Key_F4,
		Key_F5,
		Key_F6,
		Key_F7,
		Key_F8,
		Key_F9,
		Key_F10,
		Key_F11,
		Key_F12,
		Key_F13,
		Key_F14,
		Key_F15,
		Key_PrintScreen,
		Key_ScrollLock,
		Key_Pause,
		Key_Insert,
		Key_Delete,
		Key_Home,
		Key_End,
		Key_PageUp,
		Key_PageDown,
		Key_Application,
		Key_LShift,
		Key_RShift,
		Key_LCtrl,
		Key_RCtrl,
		Key_LAlt,
		Key_RAlt,

		NumButtons,
	};

	class InputStateKeyboard
	{
		friend class InputDeviceKeyboardMouse;

	public:
		InputStateKeyboard() 
			:m_KeyboardState{ SDL_GetKeyboardState(&m_KeyLength) }
		{
			m_PrevKeyboardState.fill(0);
		};

		~InputStateKeyboard()
		{
		};


		// Was the button just pressed (i.e. went from up to down this frame)
		bool WasPressed(SDL_Scancode button) const { return !m_PrevKeyboardState[button] && m_KeyboardState[button]; }

		// Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
		bool WasReleased(SDL_Scancode button) const { return m_PrevKeyboardState[button] && !m_KeyboardState[button]; }

		// Is the button being held down?
		bool IsHeldDown(SDL_Scancode button) const { return m_KeyboardState[button]; }

		// Syntactic Sugar
		//-------------------------------------------------------------------------

		bool WasEnterPressed() const { return 0; }
		bool WasEnterReleased() const { return 0; }
		bool IsEnterPressed() const { return 0; }

		bool WasShiftPressed() const { return 0; }
		bool WasShiftReleased() const { return 0; }
		bool IsShiftHeldDown() const { return 0; }

		bool WasCtrlPressed() const { return 0; }
		bool WasCtrlReleased() const { return  0; }
		bool IsCtrlHeldDown() const { return 0; }

		bool WasAltPressed() const { return 0; }
		bool WasAltReleased() const { return 0; }
		bool IsAltHeldDown() const { return 0; }

	private:

		void UpdatePreviousKeyboardState()
		{
			std::copy(m_KeyboardState, m_KeyboardState + m_KeyLength, m_PrevKeyboardState.begin());
		}

		void ProcessInput(Seconds deltaTime)
		{
			(deltaTime);


			if (WasReleased(SDL_SCANCODE_K))
			{
				std::cout << "hey";
			}

			
		};



	private:

		int  m_KeyLength;
		const Uint8* m_KeyboardState; 
		std::array<Uint8, SDL_NUM_SCANCODES> m_PrevKeyboardState;

	};
}