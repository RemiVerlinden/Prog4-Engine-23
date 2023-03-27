#pragma once
#include "glm\glm.hpp"
#include "Time.h"
#include <SDL.h>
#include "ImguiWrapper.h"
#include "SceneManager.h"
#include <iostream>
#include <array>
#include "InputState.h"

namespace dae::Input
{

	class InputStateKeyboard : public InputState
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
		virtual bool WasPressed(deviceButton button) const override
		{ 
			return !m_PrevKeyboardState[GetSDL_Scancode(button)] && m_KeyboardState[GetSDL_Scancode(button)];
		}

		// Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
		virtual bool WasReleased(deviceButton button) const override
		{ 
			return m_PrevKeyboardState[GetSDL_Scancode(button)] && !m_KeyboardState[GetSDL_Scancode(button)];
		}

		// Is the button being held down?
		virtual bool IsHeldDown(deviceButton button)  const override
		{ 
			return m_KeyboardState[GetSDL_Scancode(button)];
		}

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

		inline SDL_Scancode GetSDL_Scancode(deviceButton button) const
		{
			const int buttonType = static_cast<int>(DeviceButtonType::Keyboard);
			return std::get<buttonType>(button);
		}

		void UpdatePreviousKeyboardState()
		{
			std::copy(m_KeyboardState, m_KeyboardState + m_KeyLength, m_PrevKeyboardState.begin());
		}

		void ProcessInput(Seconds deltaTime)
		{
			(deltaTime);
			// nothing needed here because SDL is cool like that
			// all I had to do was storing the SDL_GetKeyboardState pointer and now when SDL_PollEvent gets called
			// then m_KeyboardState pointer will update 
		};



	private:

		int  m_KeyLength;
		const Uint8* m_KeyboardState; 
		std::array<Uint8, SDL_NUM_SCANCODES> m_PrevKeyboardState;

	};
}