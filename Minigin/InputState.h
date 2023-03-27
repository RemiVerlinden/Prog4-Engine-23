#pragma once
#include <variant>
#include <SDL.h>


namespace dae::Input
{
	using deviceButton = std::variant<int, SDL_Scancode, uint8_t>;

	enum class DeviceButtonType : int
	{
		Gamepad,
		Keyboard,
		Mouse
	};

	class InputState
	{
	public:

		virtual ~InputState() = default;

		// Was the button just pressed (i.e. went from up to down this frame)
		virtual bool WasPressed(deviceButton button) const = 0;


		// Was the button just release (i.e. went from down to up this frame). Also optionally returns how long the button was held for
		virtual bool WasReleased(deviceButton button) const = 0;


		// Is the button being held down?
		virtual bool IsHeldDown(deviceButton button) const = 0;
	protected:
		InputState() = default;
	};
}