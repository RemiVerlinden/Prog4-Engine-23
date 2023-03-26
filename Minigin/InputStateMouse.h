#pragma once
#include "glm\glm.hpp"
#include "Time.h"

namespace dae::Input
{

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


		inline glm::vec2 GetMovementDelta() const { return m_MovementDelta; }
		inline float GetWheelDelta(WheelType wheelType = WheelType::Vertical) const { return (wheelType == WheelType::Vertical) ? m_verticalWheelDelta : m_horizontalWheelDelta; }
		inline glm::ivec2 GetCursorPosition() const { return m_Position; }

		// Was the button just pressed (i.e. went from up to down this frame)
		// Use SDL_BUTTON for input
		bool WasPressed(uint8_t button) const { return SDL_BUTTON(button) & m_ButtonsPressedThisFrame; }

		// Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
		// Use SDL_BUTTON for input
		bool WasReleased(uint8_t button) const { return SDL_BUTTON(button) & m_ButtonsReleasedThisFrame; }

		// Is the button being held down? Use SDL_BUTTON for input
		bool IsHeldDown(uint8_t button) const { return m_MouseState & SDL_BUTTON(button); }

		void ProcesMouseWheel(SDL_MouseWheelEvent& mouseWheelEvent)
		{
			m_verticalWheelDelta = mouseWheelEvent.preciseY;
			m_horizontalWheelDelta = mouseWheelEvent.preciseX;
		}
	private:


		void ProcessInput(Seconds deltaTime)
		{
			(deltaTime);
			std::swap(m_MouseState, m_PrevMouseState);
			std::swap(m_Position, m_PrevPosition);
			m_MouseState = static_cast<uint8_t>(SDL_GetMouseState(&m_Position.x, &m_Position.y));

			uint8_t buttonChanges = m_MouseState ^ m_PrevMouseState;
			m_ButtonsPressedThisFrame = buttonChanges & m_MouseState;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_MouseState);


			m_MovementDelta = m_Position - m_PrevPosition;

			if (WasReleased(SDL_BUTTON_LEFT))
			{
				std::cout << "hey";
			}
			if (WasPressed(SDL_BUTTON_RIGHT))
			{
				std::cout << "hey";
			}
			if (IsHeldDown(SDL_BUTTON_MIDDLE))
			{
				std::cout << "npoooo";
			}

			if (IsHeldDown(SDL_BUTTON_X1))
			{
				std::cout << m_Position.x<< std::endl;
			}
		};

	private:

		glm::vec2                                 m_MovementDelta;

		glm::ivec2                                m_Position;
		glm::ivec2                                m_PrevPosition;

		uint8_t                                  m_MouseState;
		uint8_t                                  m_PrevMouseState;

		uint8_t                                  m_ButtonsPressedThisFrame;
		uint8_t                                  m_ButtonsReleasedThisFrame;

		float                                   m_verticalWheelDelta = 0;
		float                                   m_horizontalWheelDelta = 0;
	};
}