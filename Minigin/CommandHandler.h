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


	class CommandHandler final
	{
	public:
		CommandHandler() = default;
		void Update(Seconds elapsedTime);
		void BindNewAction(deviceButton button, InputAction& inputAction);
	private:
		void UpdateActionBindings();
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

		using DeviceActionBinds = std::unordered_map<deviceButton, InputAction, deviceButtonHash>;
		 std::unordered_map<int, DeviceActionBinds> m_ActionBindPerDevice;
	};
}

