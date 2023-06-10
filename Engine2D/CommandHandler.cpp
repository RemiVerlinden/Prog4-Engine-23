#include "CommandHandler.h"
#include "InputDeviceGamepad.h"
#include "InputDeviceMouse.h"
#include "InputDeviceKeyboard.h"
#include "InputState.h"

void engine::Input::CommandHandler::Update(Seconds elapsedTime)
{
	for (auto& deviceActionBindsPair : m_ActionBindPerDevice)
	{
		DeviceActionBinds& deviceActionBinds = deviceActionBindsPair.second;

		for (auto& actionBind : deviceActionBinds)
		{
			deviceButton button = actionBind.first;
			const InputAction& inputAction = actionBind.second;

			const InputState& deviceInputState = inputAction.device->GetDeviceInputState();

			bool executeCommand = false;

			switch (inputAction.pressType)
			{
				case ButtonPressType::Press:
					executeCommand = deviceInputState.WasPressed(button);
					break;
				case ButtonPressType::Hold:
					executeCommand = deviceInputState.IsHeldDown(button);
					break;
				case ButtonPressType::Release:
					executeCommand = deviceInputState.WasReleased(button);
					break;
			}

			Command* command = inputAction.command.get();

			if (executeCommand) command->Execute(elapsedTime);
		}
	}

}

void engine::Input::CommandHandler::BindNewAction(deviceButton button, InputAction& inputAction)
{
	int deviceID = inputAction.device->GetID();


	if (auto it = m_ActionBindPerDevice.find(deviceID); it == m_ActionBindPerDevice.end())
	{
		m_ActionBindPerDevice.emplace(deviceID, DeviceActionBinds{});
	}

	DeviceActionBinds& deviceBinds = m_ActionBindPerDevice[deviceID];

	deviceBinds.emplace(std::make_pair(button, std::move(inputAction)));

}

void engine::Input::CommandHandler::UnbindAction(deviceButton button, InputAction& inputAction)
{
	int deviceID = inputAction.device->GetID();


	if (auto it = m_ActionBindPerDevice.find(deviceID); it == m_ActionBindPerDevice.end())
	{
		m_ActionBindPerDevice.emplace(deviceID, DeviceActionBinds{});
	}

	DeviceActionBinds& deviceBinds = m_ActionBindPerDevice[deviceID];

	deviceBinds.emplace(std::make_pair(button, std::move(inputAction)));

}


//void engine::Input::CommandHandler::UpdateActionsCommandsList(InputActionCommandList& actionCommandList)
//{
//	m_ActionCommandList = actionCommandList;
//	UpdateActionBindings();
//}
//
//void engine::Input::CommandHandler::UpdateActionBindings()
//{
//	//for (auto& actionCommandPair : *m_ActionCommandList.GetList())
//	//{
//	//	//m_BindActions.emplace(std::make_pair(actionCommandPair.first, actionCommandPair.second.get()));
//	//}
//}
