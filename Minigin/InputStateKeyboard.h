#pragma once
#include "glm\glm.hpp"
#include "Time.h"
#include <SDL.h>
#include "ImguiWrapper.h"
#include "SceneManager.h"
#include <iostream>
#include <array>
#include "InputState.h"
#include <cstring>
namespace dae::Input
{

	class InputStateKeyboard final : public InputState
	{
		friend class InputDeviceKeyboard;

	public:
		InputStateKeyboard()
		{
			m_KeyboardState.fill(0);
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
			return static_cast<SDL_Scancode>(std::get<buttonType>(button));
		}


		void ProcessInput(Seconds deltaTime)
		{
			(deltaTime);
			std::swap(m_KeyboardState, m_PrevKeyboardState);

			const Uint8* keyboardState = SDL_GetKeyboardState(&m_KeyLength);

			if (m_KeyLength <= SDL_NUM_SCANCODES)
				std::memcpy(m_KeyboardState.data(), keyboardState, m_KeyLength * sizeof(Uint8));
		};



	private:

		int  m_KeyLength;
		std::array<Uint8, SDL_NUM_SCANCODES> m_KeyboardState;
		std::array<Uint8, SDL_NUM_SCANCODES> m_PrevKeyboardState;

	};
}