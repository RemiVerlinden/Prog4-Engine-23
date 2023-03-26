#pragma once
#include <list>
#include <unordered_map>
#include "Command.h"
#include "InputDevice.h"
#include "Time.h"
#include "InputSystem.h"

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
		uint64_t button;
		ButtonPressType pressType;
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
		void UpdateActionsCommandsList(InputActionCommandList& actionCommandList);
	private:
		void UpdateActionBindings();
	private:
		//std::unordered_map<InputAction, Command*> m_BindActions;
		std::unordered_map<int, Command*> m_BindActions{};
		InputActionCommandList m_ActionCommandList;
		std::list<InputActionCommandList> m_inputSchemes;
	};
}

