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

	class InputActionCommandList
	{
	public:
		InputActionCommandList() = default;

		//void AddInputActionCommandBind(InputDevice* device, uint64_t button, ButtonPressType pressType, std::unique_ptr<Command>& command) 
		//{
		//	//InputAction inputAction{ device, button, pressType };
		//	//m_InputActioncommandList.push_back(std::move((std::make_pair(inputAction, std::move(command)))));
		//};

		//std::list<std::pair<InputAction, std::unique_ptr<Command> >>* GetList() { return &m_InputActioncommandList; };
	private:
		//std::list<std::pair<InputAction, std::unique_ptr<Command> >> m_InputActioncommandList;
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

		std::unordered_map<deviceButton, InputAction, deviceButtonHash> m_BindActions{};
	};
}

