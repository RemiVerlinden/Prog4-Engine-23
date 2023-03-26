#include "CommandHandler.h"
#include "InputDeviceGamepad.h"
#include "InputDeviceKeyboardMouse.h"
#include "InputState.h"

void dae::Input::CommandHandler::Update(Seconds elapsedTime)
{
	for (auto& bindAction : m_BindActions)
	{
		deviceButton button = bindAction.first;
		const InputAction& inputAction = bindAction.second;

		const InputState* deviceInputState{};

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
		}

		bool executeCommand = false;

		switch (bindAction.second.pressType)
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

void dae::Input::CommandHandler::BindNewAction(deviceButton button, InputAction& inputAction)
{
	std::pair<deviceButton, InputAction> bindAction{ std::make_pair(button, std::move(inputAction)) };
	m_BindActions.emplace(std::move(bindAction));
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
