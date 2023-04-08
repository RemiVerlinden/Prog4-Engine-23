#pragma once
#include "glm\glm.hpp"
#include "Time.h"
#include "InputState.h"

namespace dae::Input
{

	//-------------------------------------------------------------------------

	class InputStateMouse final : public InputState
	{
		friend class InputDeviceMouse;

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
		virtual bool WasPressed(deviceButton button) const override { return SDL_BUTTON(GetMouseButton(button)) & m_ButtonsPressedThisFrame; }

		// Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
		// Use SDL_BUTTON for input
		virtual bool WasReleased(deviceButton button) const override { return SDL_BUTTON(GetMouseButton(button)) & m_ButtonsReleasedThisFrame; }

		// Is the button being held down? Use SDL_BUTTON for input
		virtual bool IsHeldDown(deviceButton button) const override { return SDL_BUTTON(GetMouseButton(button)) & m_MouseState; }

		void ProcesMouseWheel(const SDL_MouseWheelEvent& mouseWheelEvent)
		{
			m_verticalWheelDelta = mouseWheelEvent.preciseY;
			m_horizontalWheelDelta = mouseWheelEvent.preciseX;
		}
	private:

		inline uint8_t GetMouseButton(deviceButton button) const
		{
			const int buttonType = static_cast<int>(DeviceButtonType::Mouse);
			return std::get<buttonType>(button);
		}

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