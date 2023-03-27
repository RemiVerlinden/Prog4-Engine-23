#pragma once
#include <list>
#include <unordered_map>
#include "Command.h"
#include "InputDevice.h"
#include "Time.h"
#include "InputState.h"

namespace dae::Input
{
	enum class ButtonPressType
	{
		Press,
		Hold,
		Release
	};


	struct InputAction
	{
		InputDevice* device;
		ButtonPressType pressType;
		std::unique_ptr<Command> command;
	};


	class CommandHandler final // this is where the magic happends
	{
	public:
		CommandHandler() = default;
		void Update(Seconds elapsedTime);
		void BindNewAction(deviceButton button, InputAction& inputAction);
	private:

		struct deviceButtonHash
		{
			std::size_t operator()(const deviceButton& button) const
			{
				return std::visit([](const auto& arg)
				{
					return std::hash<std::decay_t<decltype(arg)>>()(arg);
				}, button);
			}
		};

		// DeviceActionBinds -> unoredered map where the key value is a button of any type: xinput, sdl_scancode or sdl_mouse_code
		// example of possible keys: XINPUT_GAMEPAD_A - SDL_SCANCODE_F - SDL_BUTTON_LEFT
		
		using DeviceActionBinds = std::unordered_map<deviceButton, InputAction, deviceButtonHash>;
		 std::unordered_map<int, DeviceActionBinds> m_ActionBindPerDevice;
	};
}

// Currently no way to remove binds
// 