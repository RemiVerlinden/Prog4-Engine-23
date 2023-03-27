#include "CommandHandler.h"
#include "InputDeviceGamepad.h"
#include "InputDeviceKeyboardMouse.h"
#include "InputState.h"

void dae::Input::CommandHandler::Update(Seconds elapsedTime)
{
	for (auto& deviceActionBindsPair : m_ActionBindPerDevice)
	{
		DeviceActionBinds& deviceActionBinds = deviceActionBindsPair.second;

		for (auto& actionBind : deviceActionBinds)
		{

			deviceButton button = actionBind.first;
			const InputAction& inputAction = actionBind.second;

			const InputState* deviceInputState = nullptr;

			switch (inputAction.device->GetDeviceCategory())
			{
				case DeviceCategory::Gamepad:
				{
					deviceInputState = &static_cast<InputDeviceGamepad*>(inputAction.device)->GetGamepadState();
					break;
				}
				case DeviceCategory::KeyboardMouse:
				{
					DeviceButtonType buttonType = ((std::holds_alternative<SDL_Scancode>(button)) ? DeviceButtonType::Keyboard : DeviceButtonType::Mouse);

					switch (buttonType)
					{
						case DeviceButtonType::Keyboard:
							deviceInputState = &static_cast<InputDeviceKeyboardMouse*>(inputAction.device)->GetKeyboardState();
							break;

						case DeviceButtonType::Mouse:
							deviceInputState = &static_cast<InputDeviceKeyboardMouse*>(inputAction.device)->GetMouseState();
							break;
					}
					break;
				}
				default:
					deviceInputState = &static_cast<InputDeviceKeyboardMouse*>(inputAction.device)->GetMouseState();
					break;
			}

			bool executeCommand = false;

			switch (inputAction.pressType)
			{
				case ButtonPressType::Press:
					executeCommand = deviceInputState->WasPressed(button);
					break;
				case ButtonPressType::Hold:
					executeCommand = deviceInputState->IsHeldDown(button);
					break;
				case ButtonPressType::Release:
					executeCommand = deviceInputState->WasReleased(button);
					break;
			}

			Command* command = inputAction.command.get();

			if (executeCommand) command->Execute(elapsedTime);
		}
	}

}

void dae::Input::CommandHandler::BindNewAction(deviceButton button, InputAction& inputAction)
{
	int deviceID = inputAction.device->GetID();


	if (auto it = m_ActionBindPerDevice.find(deviceID); it == m_ActionBindPerDevice.end())
	{
		m_ActionBindPerDevice.emplace(deviceID, DeviceActionBinds{});
	}

	DeviceActionBinds& deviceBinds = m_ActionBindPerDevice[deviceID];

	deviceBinds.emplace(std::move(std::make_pair(button, std::move(inputAction))));

}


//void dae::Input::CommandHandler::UpdateActionsCommandsList(InputActionCommandList& actionCommandList)
//{
//	m_ActionCommandList = actionCommandList;
//	UpdateActionBindings();
//}
//
//void dae::Input::CommandHandler::UpdateActionBindings()
//{
//	//for (auto& actionCommandPair : *m_ActionCommandList.GetList())
//	//{
//	//	//m_BindActions.emplace(std::make_pair(actionCommandPair.first, actionCommandPair.second.get()));
//	//}
//}
